// Fill out your copyright notice in the Description page of Project Settings.


#include "Service/DSBTService_UpdateDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Character/Enemy/DSEnemyCharacterBase.h"

void UDSBTService_UpdateDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	if (!AIController || !TargetActor)
	{
		return;
	}

	float Distance = FVector::Dist(Pawn->GetActorLocation(), TargetActor->GetActorLocation());
	ADSEnemyCharacterBase* Enemy = Cast<ADSEnemyCharacterBase>(Pawn);
	if (!Enemy)
	{
		return;
	}
	float AttackRange = Enemy->GetAttackStartRange();
	float ChaseRange = Enemy->GetChaseStartRange();
	bCanAttack = Distance <= AttackRange;
	bCanChase = Distance <= ChaseRange;

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("DistanceToTarget", Distance);
	// BB에서 변수끼리 비교가 안되어서 임시방편으로 bool 사용
	OwnerComp.GetBlackboardComponent()->SetValueAsBool("bCanAttack", bCanAttack);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool("bCanChase", bCanChase);
}
