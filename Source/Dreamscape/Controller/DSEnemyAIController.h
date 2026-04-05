// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DSEnemyAIController.generated.h"

/**
 * 
 */
namespace BBKeys
{
	const FName TargetActor = TEXT("TargetActor");
	const FName IsInAttackRange = TEXT("IsInAttackRange");
	const FName EnemyState = TEXT("State");
}

UCLASS()
class DREAMSCAPE_API ADSEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	void BeginPlay() override;

	void OnPossess(APawn* InPawn) override;

	void Tick(float DeltaTime) override;
	
public:
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<class UBehaviorTree> BehaviorTreeAsset;

protected:
	void UpdateBlackboardValues(class ADSEnemyCharacterBase* EnemyCharacter, APawn* PlayerPawn);

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardComponent> BBComponent;
};
