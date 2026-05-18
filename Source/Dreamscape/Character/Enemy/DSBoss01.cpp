// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/DSBoss01.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/DSBossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Animation/DSEnemyAnimInstance.h"

ADSBoss01::ADSBoss01()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
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
}

void ADSBoss01::PerformAttackTrace()
{
	UE_LOG(LogTemp, Log, TEXT("Boss Attack Trace Performing"));
}

void ADSBoss01::EndAttackTrace()
{
	UE_LOG(LogTemp, Log, TEXT("Boss Attack Trace Ended"));
}

void ADSBoss01::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UDSEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &ADSBoss01::OnAttackMontageEnded);
	}
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
