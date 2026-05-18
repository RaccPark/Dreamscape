// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/DSEnemyCharacterBase.h"
#include "Interface/DSAttackTraceInterface.h"
#include "DSBoss01.generated.h"

/**
 * 
 */
// 구조체를 사용해보자
USTRUCT(BlueprintType)
struct FBossAction
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FName ActionName;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AnimMotage;

	// 거리 기반 공격
	UPROPERTY(EditAnywhere)
	float Range;

	UPROPERTY(EditAnywhere)
	float Weight;
};

UCLASS()
class DREAMSCAPE_API ADSBoss01 : public ADSEnemyCharacterBase, public IDSAttackTraceInterface
{
	GENERATED_BODY()
public:
	ADSBoss01();

	virtual void Attack() override;

	virtual void StartAttackTrace() override;
	virtual void PerformAttackTrace() override;
	virtual void EndAttackTrace() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Boss|Action")
	TArray<FBossAction> Actions;

	UPROPERTY(VisibleAnywhere, Category = "Animation")
	TObjectPtr<class UDSEnemyAnimInstance> AnimInstance;

	const FBossAction* SelectAction();

	void ExecuteAction(const FBossAction& Action);

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
