// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "DSBTService_UpdateDistance.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSBTService_UpdateDistance : public UBTService
{
	GENERATED_BODY()
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	bool bCanAttack = false;
	bool bCanChase = false;
};
