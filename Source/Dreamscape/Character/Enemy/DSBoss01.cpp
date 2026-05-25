// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/DSBoss01.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/DSBossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "Weapon/Data/DSWeaponItemData.h"
#include "Weapon/DSSwordWeapon.h"

#include "Animation/DSBoss01AnimInstance.h"

ADSBoss01::ADSBoss01()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ADSBoss01::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UDSEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &ADSBoss01::OnAttackMontageEnded);
	}

	if (WeaponData)
	{
		AttachWeaponBySocket(Boss01Socket::IdleSocket);
	}

	OnBossLandingFinishedDelegate.AddDynamic(this, &ADSBoss01::StartBossCombat);
}

void ADSBoss01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADSBoss01::Attack()
{
	const FBossAction* SelectedAction = SelectAction();
	if (!SelectedAction)
	{
		return;
	}

	ExecuteAction(*SelectedAction);
}

void ADSBoss01::StartAttackTrace()
{
	UE_LOG(LogTemp, Log, TEXT("Boss Attack Trace Started"));
	if (!EquippedWeapon)
	{
		return;
	}
	EquippedWeapon->StartAttackTrace();
}

void ADSBoss01::PerformAttackTrace()
{
	//UE_LOG(LogTemp, Log, TEXT("Boss Attack Trace Performing"));
	if (!EquippedWeapon)
	{
		return;
	}
	EquippedWeapon->PerformTrace();
}

void ADSBoss01::EndAttackTrace()
{
	UE_LOG(LogTemp, Log, TEXT("Boss Attack Trace Ended"));
	if (!EquippedWeapon)
	{
		return;
	}
	EquippedWeapon->EndAttackTrace();
}

const FBossAction* ADSBoss01::SelectAction()
{
	if (Actions.Num() == 0)
	{
		return nullptr;
	}

	int32 Index = FMath::RandRange(0, Actions.Num() - 1);
	return &Actions[Index];
}

void ADSBoss01::ExecuteAction(const FBossAction& Action)
{
	if (Action.AnimMotage)
	{
		PlayAnimMontage(Action.AnimMotage);
	}
}

void ADSBoss01::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	OnAttackEnded.Broadcast();
}

void ADSBoss01::AttachWeaponBySocket(FName SocketName)
{
	// 이 보스는 무기를 자유자재로 바꿔 끼우는 주체가 아니므로 복잡하게 구현하지 않음.
	if (!WeaponData)
	{
		return;
	}

	// 유효한 소켓인지 확인
	if (!GetMesh()->DoesSocketExist(SocketName))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ADSBoss01] Socket %s does not exist!"), *SocketName.ToString());
		return;
	}

	// 초기 무기가 없다면 생성
	if (!EquippedWeapon)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		EquippedWeapon = GetWorld()->SpawnActor<ADSSwordWeapon>(WeaponData->SwordWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		if (!EquippedWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ADSBoss01] Failed to spawn weapon!"));
			return;
		}

		EquippedWeapon->InitializeWeapon(WeaponData);
	}
	else
	{
		// 소켓에서 장착 해제
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

void ADSBoss01::StartBossCombat()
{
	if (AnimInstance)
	{
		UDSBoss01AnimInstance* BossAnimInstance = Cast<UDSBoss01AnimInstance>(AnimInstance);
		if (BossAnimInstance)
		{
			BossAnimInstance->bWasLanded = true;
		}
	}

	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)
	{
		return;
	}

	// BT 실행
	ADSBossAIController* BossAIController = Cast<ADSBossAIController>(AIController);
	if (!BossAIController)
	{
		return;
	}

	BossAIController->SetBehaviorTreeByOwnController();
}

void ADSBoss01::OnDeath()
{
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (ADSBossAIController* BossAIController = Cast<ADSBossAIController>(GetController()))
	{
		BossAIController->GetBlackboardComponent()->SetValueAsEnum("EnemyState", (uint8)EDSEnemyState::ES_Dead);
		BossAIController->SetActorTickEnabled(false);
	}

	if (AnimInstance)
	{
		AnimInstance->bIsDead = true;
	}

	OnDeathDelegate.Broadcast();
}
