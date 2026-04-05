// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskNodes/DSBTT_Attack.h"
#include "Character/Enemy/DSEnemyCharacterBase.h"
#include "AIController.h"


UDSBTT_Attack::UDSBTT_Attack()
{
}

EBTNodeResult::Type UDSBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	ADSEnemyCharacterBase* EnemyCharacter = Cast<ADSEnemyCharacterBase>(AIController->GetPawn());
	if (!EnemyCharacter)
	{
		return EBTNodeResult::Failed;
	}

	EnemyCharacter->OnAttackEnded.RemoveAll(this);
	EnemyCharacter->OnAttackEnded.AddLambda([this, &OwnerComp]()
	{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);	// 지연된 상태로 진행 중이던 태스크가 이제 완전히 끝났음을 알리는 함수
		});

	// Call the Attack function on the enemy character
	EnemyCharacter->Attack();

	// 공격은 애니메이션 시간이 걸리므로 'In Progress'를 반환하여 태스크를 유지
	// 이후 애니메이션이 끝나면 FinishLatentTask를 통해 Succeeded를 리턴
	return EBTNodeResult::InProgress;
}
