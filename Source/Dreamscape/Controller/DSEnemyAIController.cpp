// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/DSEnemyAIController.h"
#include "Character/Enemy/DSEnemyCharacterBase.h"
#include "Character/Enemy/DSEnemyMelee.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ADSEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ADSEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);

		// BlackboardComponent 초기화
		BBComponent = GetBlackboardComponent();
	}
	
	

	//UE_LOG(LogTemp, Warning, TEXT("BB Value: %s"), GetBlackboardComponent()->GetValueAsObject("TargetActor") ? TEXT("SET") : TEXT("NULL"));
}

void ADSEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!BBComponent)
	{
		return;
	}

	ADSEnemyCharacterBase* EnemyCharacter = Cast<ADSEnemyCharacterBase>(GetPawn());
	if (!EnemyCharacter)
	{
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		return;
	}

	UpdateBlackboardValues(EnemyCharacter, PlayerPawn);
}

void ADSEnemyAIController::UpdateBlackboardValues(ADSEnemyCharacterBase* EnemyCharacter, APawn* PlayerPawn)
{
	// Calculate distance to player and update blackboard
	float DistanceToPlayer = FVector::Dist(EnemyCharacter->GetActorLocation(), PlayerPawn->GetActorLocation());

	ADSEnemyMelee* MeleeEnemy = Cast<ADSEnemyMelee>(EnemyCharacter);
	if (!MeleeEnemy)
	{
		return;
	}
	float ChaseRange = MeleeEnemy->GetChaseStartRange(); // 추적 범위
	float AttackRange = MeleeEnemy->GetAttackStartRange(); // 공격 범위

	if (DistanceToPlayer <= ChaseRange)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Player Detected! Distance: %f"), DistanceToPlayer);
		// 탐지 범위 안일 때
		BBComponent->SetValueAsObject(BBKeys::TargetActor, PlayerPawn);

		if (DistanceToPlayer <= AttackRange)
		{
			// 공격 범위 안일 때
			BBComponent->SetValueAsBool(BBKeys::IsInAttackRange, true);
			BBComponent->SetValueAsEnum(BBKeys::EnemyState, (uint8)EDSEnemyState::ES_Attack);
		}
		else
		{
			BBComponent->SetValueAsBool(BBKeys::IsInAttackRange, false);
			BBComponent->SetValueAsEnum(BBKeys::EnemyState, (uint8)EDSEnemyState::ES_Chase);
		}
	}
	else
	{
		// 탐지 범위 밖일 때
		BBComponent->SetValueAsObject(BBKeys::TargetActor, nullptr);
		BBComponent->SetValueAsBool(BBKeys::IsInAttackRange, false);

		// 기본은 Idle 상태로 설정
		BBComponent->SetValueAsEnum(BBKeys::EnemyState, (uint8)EDSEnemyState::ES_Idle);
	}
}
