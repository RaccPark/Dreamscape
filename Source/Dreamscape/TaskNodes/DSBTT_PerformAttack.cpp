// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskNodes/DSBTT_PerformAttack.h"
#include "Character/Enemy/DSEnemyCharacterBase.h"
#include "AIController.h"

EBTNodeResult::Type UDSBTT_PerformAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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


	Enemy->OnAttackEnded.AddUObject(
		this,
		&UDSBTT_PerformAttack::OnAttackFinished
	);

	BTComponent = &OwnerComp;

	// 실제 공격부
	Enemy->Attack();

	//
	return EBTNodeResult::InProgress;
}

void UDSBTT_PerformAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return;
	}

	ADSEnemyCharacterBase* Enemy = Cast<ADSEnemyCharacterBase>(AIController->GetPawn());
	if (!Enemy)
	{
		return;
	}
	
	Enemy->OnAttackEnded.RemoveAll(this);
}

void UDSBTT_PerformAttack::OnAttackFinished()
{
	if (!BTComponent)
	{
		return;
	}

	FinishLatentTask(*BTComponent, EBTNodeResult::Succeeded);

}
