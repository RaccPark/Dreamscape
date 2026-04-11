// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/DSEnemyMelee.h"
#include "Controller/DSEnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/DSEnemyAnimInstance.h"

#include "DrawDebugHelpers.h" 

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

	// Attack 관련 설정
	MeleeSocket = "MeleeSocket";
	MeleeTraceRadius = 20.0f;
	bShowDebugTrace = false;
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

void ADSEnemyMelee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADSEnemyMelee::ApplyDamage(float DamageAmount)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0.0f)
	{
		OnDeath();
		return;
	}

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
		// 
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &ADSEnemyCharacterBase::OnAttackMontageEnded);
		EnemyAnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontage);
	}
}

void ADSEnemyMelee::UpdateWalkSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void ADSEnemyMelee::StartMeleeTrace()
{
	HitActors.Empty();	// 공격 시작 시 이미 공격이 적용된 액터 목록 초기화

	PreviousSocketLocation = GetMesh()->GetSocketLocation(MeleeSocket);
	if (!GetMesh()->DoesSocketExist(MeleeSocket))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sockets 'MeleeSocket' do not exist on the mesh!"));
	}
}

void ADSEnemyMelee::PerformMeleeTrace()
{
	FVector CurrentSocketLocation = GetMesh()->GetSocketLocation(MeleeSocket);

	// 소켓 검사
	if (!GetMesh()->DoesSocketExist(MeleeSocket))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sockets 'MeleeSocket' do not exist on the mesh!"));
	}

	TArray<FHitResult> HitResults;
	FCollisionShape ColiisionSphere = FCollisionShape::MakeSphere(MeleeTraceRadius);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);	// 자신은 충돌 검사에서 제외

	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, PreviousSocketLocation, CurrentSocketLocation, FQuat::Identity, ECC_GameTraceChannel1, ColiisionSphere, QueryParams);

	if (bShowDebugTrace)
	{
		FColor DebugColor = bHit ? FColor::Red : FColor::Green;
		DrawDebugCapsule(GetWorld(), (PreviousSocketLocation + CurrentSocketLocation) * 0.5f, FVector::Distance(PreviousSocketLocation, CurrentSocketLocation) * 0.5f, ColiisionSphere.GetSphereRadius(), FQuat::FindBetweenNormals(FVector::UpVector, CurrentSocketLocation - PreviousSocketLocation), DebugColor, false, 2.0f);
	}

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();

			if (HitActor && !HitActors.Contains(HitActor))
			{
				HitActors.Add(HitActor);

				IDSDamageableInterface* DamageableActor = Cast<IDSDamageableInterface>(HitActor);
				if (DamageableActor)
				{
					DamageableActor->ApplyDamage(AttackDamage);

					// vfx, sfx etc.
					// ...
				}
			}
		}
	}
}

void ADSEnemyMelee::EndMeleeTrace()
{
	HitActors.Empty();
}
