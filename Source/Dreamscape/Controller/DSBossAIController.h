// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DSBossAIController.generated.h"

/**
 * 
 */

UCLASS()
class DREAMSCAPE_API ADSBossAIController : public AAIController
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

	void OnPossess(APawn* InPawn) override;

	void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<class UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<class UBlackboardComponent> BBComponent;

	void TrySetPlayerAsTarget();

public:
	void SetBehaviorTreeByOwnController();

	UBehaviorTree* GetBehaviorTree() const;

};
