// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskNodes/DSBTT_SetState.h"
#include "Character/Enemy/DSEnemyCharacterBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UDSBTT_SetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	ADSEnemyCharacterBase* Enemy = Cast<ADSEnemyCharacterBase>(AIController->GetPawn());
	if (!Enemy)
	{
		return EBTNodeResult::Failed;
	}

	Enemy->SetState(StateToSet);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum("EnemyState", (uint8)StateToSet);

	return EBTNodeResult::Succeeded;
}
