// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DSBTT_PerformAttack.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSBTT_PerformAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

protected:
	UFUNCTION()
	void OnAttackFinished();

	UBehaviorTreeComponent* BTComponent;
};
