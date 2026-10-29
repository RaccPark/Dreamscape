// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DSPlayerSubAbilityComponent.h"
#include "Character/DSCharacterPlayer.h"
#include "Character/Data/DSSubAbilityData.h"
#include "Components/DSPlayerStatsComponent.h"
#include "Components/DSPlayerVitalityComponent.h"

#include "TimerManager.h"

// Sets default values for this component's properties
UDSPlayerSubAbilityComponent::UDSPlayerSubAbilityComponent()
{
	// 충전 및 회복은 타이머로 관리
	PrimaryComponentTick.bCanEverTick = false;

	SubAbilitySlots.SetNum(SubAbilitySlotCount);
	SelectedSlotIndex = 0;

	bIsCharging = false;
	bIsChargingComplete = false;
	bIsRecovering = false;
}


// Called when the game starts
void UDSPlayerSubAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ADSCharacterPlayer>(GetOwner());
	if (OwnerCharacter)
	{
		PlayerStatsComponent = OwnerCharacter->GetStatsComponent();
		PlayerVitalityComponent = OwnerCharacter->GetVitalityComponent();
	}
	
}


bool UDSPlayerSubAbilityComponent::SelectSlot(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= SubAbilitySlotCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerSubAbilityComponent] Invalid slot index: %d"), SlotIndex);
		return false;
	}

	if (SlotIndex == SelectedSlotIndex)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerSubAbilityComponent] Slot index %d is already selected"), SlotIndex);
		return true;
	}

	// 충전 중이면 취소하고 슬롯 전환
	const bool bWasCharging = bIsCharging;
	if (bWasCharging)
	{
		CancelCharge();
	}

	// 슬롯 전환
	SelectedSlotIndex = SlotIndex;

	const UDSSubAbilityData* Data = GetSelectedSubAbilityData();
	if (Data)
	{
		UE_LOG(LogTemp, Log, TEXT("[UDSPlayerSubAbilityComponent] 슬롯 %d 선택: %s"), SlotIndex + 1, *Data->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerSubAbilityComponent] 슬롯 %d 선택: 비어 있음"), SlotIndex + 1);
	}

	// 충전 중단 시 서브 공격 상태가 Idle로 돌아가도록 함
	// 충전 미완료도 같음
	if (bWasCharging)
	{
		OnSubAbilityEnded.Broadcast();
	}

	return true;
}

const UDSSubAbilityData* UDSPlayerSubAbilityComponent::GetSelectedSubAbilityData() const
{
	return SubAbilitySlots.IsValidIndex(SelectedSlotIndex) ? SubAbilitySlots[SelectedSlotIndex].Get() : nullptr;
}

int32 UDSPlayerSubAbilityComponent::GetSelectedSlotIndex() const
{
	return SelectedSlotIndex;
}

bool UDSPlayerSubAbilityComponent::CanBeginCharge() const
{
	const UDSSubAbilityData* Data = GetSelectedSubAbilityData();
	if (!Data || !PlayerVitalityComponent)
	{
		return false;
	}

	if (bIsCharging || bIsRecovering)
	{
		return false;
	}

	if (!PlayerVitalityComponent->CanConsumeStamina(Data->StaminaCost))
	{
		UE_LOG(LogTemp, Log, TEXT("[UDSPlayerSubAbilityComponent] 스테미나 부족: 필요 %.1f, 현재 %.2f"),
			Data->StaminaCost, PlayerVitalityComponent->GetCurrentStamina());
		return false;
	}

	return true;
}

void UDSPlayerSubAbilityComponent::BeginCharge()
{
	if (!GetSelectedSubAbilityData())
	{
		return;
	}

	bIsCharging = true;
	bIsChargingComplete = false;

	const float ChargeTime = GetChargeTime();
	if (ChargeTime <= 0.0f)
	{
		OnChargeComplete();
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &UDSPlayerSubAbilityComponent::OnChargeComplete, ChargeTime, false);

	UE_LOG(LogTemp, Log, TEXT("[UDSPlayerSubAbilityComponent] 충전 시작 (%.2f초)"), ChargeTime);
}

void UDSPlayerSubAbilityComponent::Release()
{
	if (!bIsCharging)
	{
		return;
	}

	// 충전이 덜 찼으면 취소
	// 스테미나 소모하지 않음
	if (!bIsChargingComplete)
	{
		CancelCharge();
		UE_LOG(LogTemp, Log, TEXT("[UDSPlayerSubAbilityComponent] 충전 미완료 - 취소"));
		OnSubAbilityEnded.Broadcast();
		return;
	}

	const UDSSubAbilityData* Data = GetSelectedSubAbilityData();
	CancelCharge();

	// 충전 시작 때 확인 후 상태가 바뀌었을 수 있으니 소모 결과(ConsumeStamina)로 한 번 더 확인
	if (!Data || !PlayerVitalityComponent || !PlayerVitalityComponent->ConsumeStamina(Data->StaminaCost))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerSubAbilityComponent] 발동 실패 - 데이터 없음 또는 스테미나 부족"));
		OnSubAbilityEnded.Broadcast();
		return;
	}

	ActivateSubAbility(Data);
	StartRecovery();
}

void UDSPlayerSubAbilityComponent::CancelCharge()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ChargeTimerHandle);
	}

	bIsCharging = false;
	bIsChargingComplete = false;
}

bool UDSPlayerSubAbilityComponent::IsCharging() const
{
	return bIsCharging;
}

bool UDSPlayerSubAbilityComponent::IsChargingComplete() const
{
	return bIsChargingComplete;
}

bool UDSPlayerSubAbilityComponent::IsRecovering() const
{
	return bIsRecovering;
}

void UDSPlayerSubAbilityComponent::OnChargeComplete()
{
	bIsChargingComplete = true;
	ChargeTimerHandle.Invalidate();

	UE_LOG(LogTemp, Log, TEXT("[UDSPlayerSubAbilityComponent] 충전 완료"));
}

void UDSPlayerSubAbilityComponent::ActivateSubAbility(const UDSSubAbilityData* Data)
{
	if (!Data)
	{
		return;
	}

	switch (Data->SubAbilityType)
	{
	case EDSSubAbilityType::Arrow:
		ActivateArrow(Data);
		break;

		// cone

		// explosion

		// dash

	default:
		UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerSubAbilityComponent] 아직 구현되지 않은 보조 공격: %s"), *Data->GetName());
		break;
	}
}

void UDSPlayerSubAbilityComponent::ActivateArrow(const UDSSubAbilityData* Data)
{
	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerSubAbilityComponent] 화살 발동: %s (투사체는 추후 구현)"), *Data->GetName());
}

void UDSPlayerSubAbilityComponent::StartRecovery()
{
	bIsRecovering = true;

	const float RecoveryTime = GetRecoveryTime();
	if (RecoveryTime <= 0.0f)
	{
		OnRecoveryComplete();
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(RecoveryTimerHandle, this, &UDSPlayerSubAbilityComponent::OnRecoveryComplete, RecoveryTime, false);
}

void UDSPlayerSubAbilityComponent::OnRecoveryComplete()
{
	bIsRecovering = false;
	RecoveryTimerHandle.Invalidate();

	OnSubAbilityEnded.Broadcast();
}

float UDSPlayerSubAbilityComponent::GetChargeTime() const
{
	const UDSSubAbilityData* Data = GetSelectedSubAbilityData();
	if (!Data || !PlayerStatsComponent)
	{
		return Data ? Data->ChargeTime : 0.0f;
	}

	// 배수가 0 이하인 경우는 Stats getter가 1.0으로 막는다
	return Data->ChargeTime / PlayerStatsComponent->GetSubAttackChargeRateMultiplier();
}

float UDSPlayerSubAbilityComponent::GetRecoveryTime() const
{
	return PlayerStatsComponent ? PlayerStatsComponent->GetSubAttackRecoveryTime() : 0.0f;
}



