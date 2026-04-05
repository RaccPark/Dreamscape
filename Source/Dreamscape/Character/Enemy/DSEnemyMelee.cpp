// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/DSEnemyMelee.h"
#include "Controller/DSEnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/DSEnemyAnimInstance.h"

ADSEnemyMelee::ADSEnemyMelee()
{
	AttackDamage = 1.0f;

	// 이후 이것도 PrimaryDataAsset에서 설정할 수 있도록 변경
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);

	// AI Controller 설정
	AIControllerClass = ADSEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ADSEnemyMelee::BeginPlay()
{
	Super::BeginPlay();
}

void ADSEnemyMelee::OnDeath()
{
	Super::OnDeath();

	ADSEnemyAIController* EnemyAIController = Cast<ADSEnemyAIController>(GetController());
	if (EnemyAIController)
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsEnum(BBKeys::EnemyState, (uint8)EDSEnemyState::ES_Dead);
		EnemyAIController->SetActorTickEnabled(false);
	}

	SetActorTickEnabled(false);

}

void ADSEnemyMelee::ApplyDamage(float DamageAmount)
{
	if (HitReactMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Playing hit react montage!"));
		EnemyAnimInstance->Montage_Stop(0.1f, AttackMontage);
		PlayAnimMontage(HitReactMontage);
	}

	// 블랙 보드에 피격 상태 업데이트
	if (ADSEnemyAIController* EnemyAIController = Cast<ADSEnemyAIController>(GetController()))
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsHitted"), true);

		// 타이머를 설정하여 일정 시간 후에 피격 상태를 false로 변경
		GetWorldTimerManager().SetTimer(HitTimerHandle, [EnemyAIController]()
		{
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsHitted"), false);
			}, 0.5f, false); // 0.5초 후에 피격 상태 해제
	}

	TakeDamage(DamageAmount);
}

void ADSEnemyMelee::ApplyDamageWithKnockback(float DamageAmount, const FVector& KnockbackDirection, float KnockbackStrength)
{
	ApplyDamage(DamageAmount);
	LaunchCharacter(KnockbackDirection * KnockbackStrength, true, true);
}

void ADSEnemyMelee::Attack()
{
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}
	
	if (EnemyAnimInstance && AttackMontage)
	{
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &ADSEnemyCharacterBase::OnAttackMontageEnded);
		EnemyAnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontage);
	}
}

void ADSEnemyMelee::UpdateWalkSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}
