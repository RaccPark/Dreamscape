// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Types/DSEnemyState.h"
#include "DSBTT_SetState.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSBTT_SetState : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	EDSEnemyState StateToSet;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
