// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DSPlayerCombatComponent.h"
#include "Components/DSPlayerStatsComponent.h"
#include "Character/DSCharacterPlayer.h"
#include "Character/DSCharacterComboActionData.h"
#include "Weapon/DSSwordWeaponBase.h"
#include "Weapon/Data/DSWeaponItemData.h"
#include "Interface/DSDamageableInterface.h"
#include "Character/Data/DSSubAbilityData.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

UDSPlayerCombatComponent::UDSPlayerCombatComponent()
{
	// 판정은 애님 노티파이가 틱마다 불러준다. 컴포넌트 자체 틱은 필요 없다.
	PrimaryComponentTick.bCanEverTick = false;

	KnockbackStrength = 250.0f;	// 종전에 무기에 하드코딩되어 있던 값
	bDrawWeaponTrace = false;

	CurrentCombo = 0;
	bComboWindowOpen = false;
	bIsRecovering = false;
}

void UDSPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ADSCharacterPlayer>(GetOwner());
	if (OwnerCharacter)
	{
		StatsComponent = OwnerCharacter->GetStatsComponent();
	}
}

// ==================================================
// 무기
// ==================================================

void UDSPlayerCombatComponent::EquipWeapon(const UDSWeaponItemData* NewWeaponData)
{
	if (!NewWeaponData || !OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerCombatComponent] EquipWeapon: 무기 데이터 또는 소유자가 없습니다."));
		return;
	}

	// 기존 무기 해제
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
		EquippedWeapon = nullptr;
	}

	EquippedWeaponData = NewWeaponData;

	if (!NewWeaponData->SwordWeaponClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerCombatComponent] %s에 SwordWeaponClass가 없습니다."), *NewWeaponData->GetName());
		return;
	}

	// 새 무기를 등 뒤(Idle) 소켓에 스폰
	const FName SocketName = TEXT("WeaponIdleSocket");
	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	const FVector SpawnLocation = Mesh->GetSocketLocation(SocketName);
	const FRotator SpawnRotation = Mesh->GetSocketRotation(SocketName);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.Instigator = OwnerCharacter->GetInstigator();

	ADSSwordWeaponBase* SpawnedWeapon = GetWorld()->SpawnActor<ADSSwordWeaponBase>(NewWeaponData->SwordWeaponClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (SpawnedWeapon)
	{
		SpawnedWeapon->InitializeWeapon(NewWeaponData);
		SpawnedWeapon->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}

	EquippedWeapon = SpawnedWeapon;
	if (EquippedWeapon)
	{
		EquippedWeapon->SetDrawTrace(bDrawWeaponTrace);
	}
	UE_LOG(LogTemp, Log, TEXT("[UDSPlayerCombatComponent] 무기 장착: %s"), *NewWeaponData->GetName());
}

void UDSPlayerCombatComponent::SetWeaponSocket(FName SocketName)
{
	if (!EquippedWeapon || !OwnerCharacter)
	{
		return;
	}

	EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	EquippedWeapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

// ==================================================
// 파생값
// ==================================================

float UDSPlayerCombatComponent::CalculateDamage() const
{
	if (!StatsComponent || !EquippedWeaponData)
	{
		return 0.0f;
	}

	return StatsComponent->GetAttackPower() * EquippedWeaponData->DamageMultiplier;
}

float UDSPlayerCombatComponent::GetAttackPlayRate() const
{
	const float WeaponRate = EquippedWeaponData ? EquippedWeaponData->AttackRate : 1.0f;
	const float StatRate = StatsComponent ? StatsComponent->GetAttackSpeedMultiplier() : 1.0f;

	return FMath::Max(0.01f, WeaponRate * StatRate);
}

float UDSPlayerCombatComponent::GetRecoveryTime() const
{
	const float WeaponRecovery = EquippedWeaponData ? EquippedWeaponData->RecoveryTime : 0.0f;
	return StatsComponent ? StatsComponent->GetAttackRecoveryTime(WeaponRecovery) : WeaponRecovery;
}

float UDSPlayerCombatComponent::CalculateSubAbilityDamage(const UDSSubAbilityData* SubAbilityData) const
{
	if (!StatsComponent || !SubAbilityData)
	{
		return 0.0f;
	}

	return StatsComponent->GetAttackPower() * SubAbilityData->DamageMultiplier;
}

float UDSPlayerCombatComponent::GetComboWindowOpenTime(int32 ComboIndex) const
{
	float Raw = 0.0f, Latest = 0.0f;
	return ComputeComboWindowOpenTime(ComboIndex, Raw, Latest);
}

float UDSPlayerCombatComponent::ComputeComboWindowOpenTime(int32 ComboIndex, float& OutRawOpenTime, float& OutLatestOpenTime, FName SectionOverride) const
{
	OutRawOpenTime = -1.0f;
	OutLatestOpenTime = -1.0f;

	const UDSCharacterComboActionData* ComboData = EquippedWeaponData ? EquippedWeaponData->ComboActionData.Get() : nullptr;
	const UAnimMontage* Montage = EquippedWeaponData ? EquippedWeaponData->AttackMontage.Get() : nullptr;
	if (!ComboData || !Montage || ComboData->FrameRate <= 0.0f)
	{
		return -1.0f;
	}

	const int32 ArrayIndex = ComboIndex - 1;
	if (!ComboData->EffectiveFrameCounts.IsValidIndex(ArrayIndex))
	{
		return -1.0f;
	}

	const float PlayRate = GetAttackPlayRate();

	// 프레임 → 애니메이션 시간 → 배속 반영한 실제 시간
	OutRawOpenTime = (ComboData->EffectiveFrameCounts[ArrayIndex] / ComboData->FrameRate) / PlayRate;

	// 블렌드 아웃은 배속과 무관한 실제 초 단위로 트리거된다 (AnimMontage.cpp: PlayTimeToEnd <= BlendOutTime).
	// 그래서 배속이 오르면 섹션 끝의 블렌드 아웃 구간이 창 열림 시각을 삼킬 수 있다
	// 이미 Stop()된 몽타주에는 JumpToSection이 먹지 않는다. 창은 반드시 블렌드 아웃 시작 전에 열려야 한다.
	// 섹션 찾기: 재생 중이면 현재 섹션(정확), 아니면 이름 규약으로. 1타는 이름이 달라도 0번 섹션이다.
	int32 SectionIndex = INDEX_NONE;
	if (SectionOverride != NAME_None)
	{
		SectionIndex = Montage->GetSectionIndex(SectionOverride);
	}
	if (SectionIndex == INDEX_NONE)
	{
		const FName SectionName = *FString::Printf(TEXT("%s%d"), *ComboData->MontageSectionNameProfix, ComboIndex);
		SectionIndex = Montage->GetSectionIndex(SectionName);
	}
	if (SectionIndex == INDEX_NONE && ComboIndex == 1)
	{
		SectionIndex = 0;
	}
	if (SectionIndex == INDEX_NONE)
	{
		return OutRawOpenTime;
	}

	// 실제 배속 = 요청 배속 × 몽타주 RateScale (엔진 서브스테퍼가 이 곱을 쓴다)
	const float EffectiveRate = PlayRate * Montage->RateScale;
	const float SectionLengthReal = Montage->GetSectionLength(SectionIndex) / FMath::Max(EffectiveRate, 0.01f);
	const float BlendOutTrigger = (Montage->BlendOutTriggerTime >= 0.0f) ? Montage->BlendOutTriggerTime : Montage->BlendOut.GetBlendTime();
	OutLatestOpenTime = FMath::Max(0.0f, SectionLengthReal - BlendOutTrigger - 0.05f);	// 타이머 지연 + 프레임 여유

	return FMath::Min(OutRawOpenTime, OutLatestOpenTime);
}

// ==================================================
// 콤보
// ==================================================

void UDSPlayerCombatComponent::ProcessComboCommand()
{
	if (!EquippedWeaponData || !EquippedWeaponData->AttackMontage || !EquippedWeaponData->ComboActionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerCombatComponent] 장착된 무기에 몽타주 또는 콤보 데이터가 없습니다."));
		return;
	}

	// 회복 중에는 어떤 입력도 받지 않는다 (기획: 공격 후 다음 행동까지의 회복 시간)
	if (bIsRecovering)
	{
		UE_LOG(LogTemp, Log, TEXT("[Command] ProcessComboCommand: 회복 중 -> 입력 무시"));
		return;
	}

	if (CurrentCombo == 0)
	{
		// 즉시 실행 — Command 생성 후 바로 Execute (PDF 구현방법1 구조)
		UE_LOG(LogTemp, Log, TEXT("[Command] ProcessComboCommand: 첫 공격 -> DSSwordAttackCommand 생성 후 즉시 Execute"));
		TUniquePtr<IDSComboCommand> Cmd = MakeUnique<DSSwordAttackCommand>();
		Cmd->Execute(this);
		return;
	}

	if (!CanAdvanceCombo())
	{
		UE_LOG(LogTemp, Log, TEXT("[Command] ProcessComboCommand: 콤보 상한(%d) 도달 -> 입력 무시"), CurrentCombo);
		return;
	}

	if (bComboWindowOpen)
	{
		// 창이 이미 열려 있다 — 즉시 다음 타로. 배속이 올라 창이 빨리 열려도 입력을 버리지 않는다.
		UE_LOG(LogTemp, Log, TEXT("[Command] ProcessComboCommand: 창 열림 -> 콤보 %d 즉시 진행"), CurrentCombo + 1);
		AdvanceCombo();
	}
	else
	{
		// 창이 아직 안 열렸다 — 버퍼에 적재, 창이 열리는 순간 ComboCheck가 소비한다
		UE_LOG(LogTemp, Log, TEXT("[Command] ProcessComboCommand: 콤보 %d 진행 중 -> PendingComboCommand 적재"), CurrentCombo);
		PendingComboCommand = MakeUnique<DSSwordAttackCommand>();
	}
}

void UDSPlayerCombatComponent::ComboActionBegin()
{
	if (!OwnerCharacter || !EquippedWeaponData)
	{
		return;
	}

	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerCombatComponent] AnimInstance가 없습니다."));
		return;
	}

	CurrentCombo = 1;
	bComboWindowOpen = false;
	PendingComboCommand.Reset();

	PlayAttackMontage(AnimInstance);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void UDSPlayerCombatComponent::PlayAttackMontage(UAnimInstance* AnimInstance)
{
	UAnimMontage* Montage = EquippedWeaponData ? EquippedWeaponData->AttackMontage.Get() : nullptr;
	if (!AnimInstance || !Montage)
	{
		return;
	}

	// 배속 = 무기 공격 속도 * 스탯 배수. 종전에는 1.0f로 고정되어 있었다.
	AnimInstance->Montage_Play(Montage, GetAttackPlayRate());

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UDSPlayerCombatComponent::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, Montage);
}

bool UDSPlayerCombatComponent::CanAdvanceCombo() const
{
	const UDSCharacterComboActionData* ComboData = EquippedWeaponData ? EquippedWeaponData->ComboActionData.Get() : nullptr;
	if (!ComboData)
	{
		return false;
	}

	return CurrentCombo > 0 && CurrentCombo < static_cast<int32>(ComboData->MaxComboCount);
}

void UDSPlayerCombatComponent::AdvanceCombo()
{
	const UDSCharacterComboActionData* ComboData = EquippedWeaponData ? EquippedWeaponData->ComboActionData.Get() : nullptr;
	UAnimInstance* AnimInstance = OwnerCharacter ? OwnerCharacter->GetMesh()->GetAnimInstance() : nullptr;
	if (!ComboData || !AnimInstance || !CanAdvanceCombo())
	{
		PendingComboCommand.Reset();
		return;
	}

	// Montage_GetIsStopped는 Stop()이 불린 순간(블렌드 아웃 "시작")부터 true다 - 인스턴스가 ActiveMontagesMap에서 빠지기 때문.
	// 종료 델리게이트는 블렌드 아웃이 "끝나야" 오므로, 그 사이에 창이 열리면 콤보는 살아 있는데 점프는 먹지 않는다.
	// 이 경우 이전 인스턴스의 종료 델리게이트를 떼고 몽타주를 다시 재생한 뒤 점프한다.
	UAnimMontage* Montage = EquippedWeaponData->AttackMontage;
	if (AnimInstance->Montage_GetIsStopped(Montage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[Command] AdvanceCombo: 몽타주가 블렌드 아웃 중 -> 재시작 후 콤보 %d 진행 (창 열림이 블렌드 아웃보다 늦었다. 몽타주 BlendOut을 줄이거나 EffectiveFrameCounts를 낮추면 사라진다)"), CurrentCombo + 1);

		// 아직 블렌드 아웃 중인 이전 인스턴스가 나중에 ComboActionEnd를 부르면 새 콤보가 끊긴다. 먼저 떼어 낸다.
		if (FAnimMontageInstance* OldInstance = AnimInstance->GetInstanceForMontage(Montage))
		{
			OldInstance->OnMontageEnded.Unbind();
		}

		PlayAttackMontage(AnimInstance);
	}

	++CurrentCombo;
	bComboWindowOpen = false;
	PendingComboCommand.Reset();
	ComboTimerHandle.Invalidate();

	const FName NextSectionName = *FString::Printf(TEXT("%s%d"), *ComboData->MontageSectionNameProfix, CurrentCombo);
	AnimInstance->Montage_JumpToSection(NextSectionName, EquippedWeaponData->AttackMontage);
	UE_LOG(LogTemp, Log, TEXT("[Command] AdvanceCombo: 콤보 %d 섹션 %s"), CurrentCombo, *NextSectionName.ToString());

	SetComboCheckTimer();
}

void UDSPlayerCombatComponent::ComboActionEnd(UAnimMontage* TargetMontage, bool bInterrupted)
{
	ensure(CurrentCombo != 0);

	// 창이 열리기도 전에 몽타주가 끝났는데 선입력이 남아 있다면 데이터 문제다.
	// EffectiveFrameCounts가 섹션 길이보다 크다는 뜻. 경고를 남기고 버린다.
	if (PendingComboCommand && !bInterrupted)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerCombatComponent] 콤보 %d: 선입력 창이 열리기 전에 섹션이 끝났습니다. EffectiveFrameCounts[%d]를 섹션 길이보다 작게 잡으세요."),
			CurrentCombo, CurrentCombo - 1);
	}

	CurrentCombo = 0;
	bComboWindowOpen = false;
	PendingComboCommand.Reset();
	ComboTimerHandle.Invalidate();

	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	// 회복 시간. 이 동안 FSM은 SwordAttack 상태에 머물러 이동,구르기,공격이 전부 막힌다.
	const float RecoveryTime = GetRecoveryTime();
	if (RecoveryTime > 0.0f)
	{
		bIsRecovering = true;
		GetWorld()->GetTimerManager().SetTimer(RecoveryTimerHandle, this, &UDSPlayerCombatComponent::OnRecoveryFinished, RecoveryTime, false);
	}
	else
	{
		OnRecoveryFinished();
	}
}

void UDSPlayerCombatComponent::OnRecoveryFinished()
{
	bIsRecovering = false;
	RecoveryTimerHandle.Invalidate();

	OnAttackSequenceEnded.Broadcast();
}

void UDSPlayerCombatComponent::SetComboCheckTimer()
{
	const UDSCharacterComboActionData* ComboData = EquippedWeaponData ? EquippedWeaponData->ComboActionData.Get() : nullptr;
	if (!ComboData)
	{
		return;
	}

	const int32 ComboIndex = CurrentCombo - 1;
	if (!ComboData->EffectiveFrameCounts.IsValidIndex(ComboIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerCombatComponent] EffectiveFrameCounts[%d]가 없습니다. 콤보 데이터를 확인하세요."), ComboIndex);
		return;
	}

	// 지금 실제로 재생 중인 섹션으로 계산한다. 섹션 이름 규약이 안 맞아도 정확하다.
	const UAnimInstance* AnimInstance = OwnerCharacter ? OwnerCharacter->GetMesh()->GetAnimInstance() : nullptr;
	const FName CurrentSection = AnimInstance ? AnimInstance->Montage_GetCurrentSection(EquippedWeaponData->AttackMontage) : NAME_None;

	float RawOpenTime = 0.0f, LatestOpenTime = 0.0f;
	const float WindowOpenTime = ComputeComboWindowOpenTime(CurrentCombo, RawOpenTime, LatestOpenTime, CurrentSection);

	if (LatestOpenTime >= 0.0f && RawOpenTime > LatestOpenTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerCombatComponent] 콤보 %d: 창 열림 %.3fs가 블렌드 아웃 시작 %.3fs보다 늦어 앞당깁니다 (배속 x%.2f). EffectiveFrameCounts[%d]를 낮추거나 몽타주 BlendOut을 줄이세요."),
			CurrentCombo, RawOpenTime, LatestOpenTime, GetAttackPlayRate(), ComboIndex);
	}

	if (WindowOpenTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UDSPlayerCombatComponent::ComboCheck, WindowOpenTime, false);
	}
	else
	{
		// 0프레임이면 즉시 열린다
		ComboCheck();
	}
}

void UDSPlayerCombatComponent::ComboCheck()
{
	// 선입력 창이 열렸다. 섹션이 끝날 때까지 열려 있다.
	ComboTimerHandle.Invalidate();
	bComboWindowOpen = true;

	if (PendingComboCommand)
	{
		UE_LOG(LogTemp, Log, TEXT("[Command] ComboCheck: 창 열림 + 선입력 있음 -> 콤보 %d 진행"), CurrentCombo + 1);
		AdvanceCombo();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[Command] ComboCheck: 창 열림, 선입력 없음 -> 입력 대기"));
	}
}

// ==================================================
// 판정 — 무기 소켓 트레이스에 위임. 무기는 닿은 액터만 돌려주고 피해는 여기서 준다
// ==================================================

void UDSPlayerCombatComponent::StartAttackTrace()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->StartAttackTrace();
	}

	// 휘두를 때마다 전방 런치. 무기 데이터 값이 0이면 하지 않는다.
	if (OwnerCharacter && EquippedWeaponData && EquippedWeaponData->LungeImpulse > 0.0f)
	{
		OwnerCharacter->LaunchCharacter(OwnerCharacter->GetActorForwardVector() * EquippedWeaponData->LungeImpulse, true, true);
	}
}

void UDSPlayerCombatComponent::PerformAttackTrace()
{
	if (!EquippedWeapon || !OwnerCharacter)
	{
		return;
	}

	// 무기가 이번 틱에 새로 닿은 액터를 돌려준다. 중복은 무기가 거른다.
	TArray<AActor*> NewHits;
	EquippedWeapon->CollectHits(NewHits);

	if (NewHits.Num() == 0)
	{
		return;
	}

	const float Damage = CalculateDamage();

	for (AActor* HitActor : NewHits)
	{
		IDSDamageableInterface* Damageable = Cast<IDSDamageableInterface>(HitActor);
		if (!Damageable)
		{
			continue;
		}

		// 넉백은 무기 위치에서 대상 쪽으로. 수평·정규화는 SetKnockbackFrom이 한다
		FDSDamageInfo DamageInfo(Damage, EDSDamageType::MainAttack, OwnerCharacter);
		DamageInfo.SetKnockbackFrom(EquippedWeapon->GetActorLocation(), HitActor, KnockbackStrength);

		Damageable->ApplyDamage(DamageInfo);

		UE_LOG(LogTemp, Log, TEXT("[UDSPlayerCombatComponent] 히트: %s  피해 %.2f"), *HitActor->GetName(), Damage);
	}
}

void UDSPlayerCombatComponent::EndAttackTrace()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->EndAttackTrace();
	}
}

void UDSPlayerCombatComponent::SetDrawWeaponTrace(bool bEnabled)
{
	bDrawWeaponTrace = bEnabled;

	if (EquippedWeapon)
	{
		EquippedWeapon->SetDrawTrace(bEnabled);
	}
}

void UDSPlayerCombatComponent::ApplySubAbilityDamage(AActor* Target, const UDSSubAbilityData* SubAbilityData, const FVector& KnockbackDirection)
{
	IDSDamageableInterface* Damageable = Cast<IDSDamageableInterface>(Target);
	if (!Damageable || !SubAbilityData)
	{
		return;
	}

	// 보조 공격은 폭발이 아니면 경직을 주지 않음.
	// 피해 종류를 여기서 가른다.
	const EDSDamageType DamageType = (SubAbilityData->SubAbilityType == EDSSubAbilityType::Explosion) ? EDSDamageType::Explosion : EDSDamageType::SubAttack;

	const float Damage = CalculateSubAbilityDamage(SubAbilityData);

	// 공격자는 투사체가 아니라 쏜 캐릭터. 수평·정규화는 SetKnockback이 한다
	FDSDamageInfo DamageInfo(Damage, DamageType, OwnerCharacter);
	DamageInfo.SetKnockback(KnockbackDirection, KnockbackStrength);

	Damageable->ApplyDamage(DamageInfo);

	UE_LOG(LogTemp, Log, TEXT("[UDSPlayerCombatComponent] 보조 공격 히트: %s  피해 %.2f  (%s)"), *Target->GetName(), Damage, *SubAbilityData->GetName());
}
