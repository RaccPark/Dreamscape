// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/DSBossCharacterBase.h"
#include "Interface/DSAttackTraceInterface.h"
#include "Interface/DSWeaponAttachInterface.h"
#include "DSBoss01.generated.h"

/**
 * 
 */
namespace Boss01Socket
{
	const FName IdleSocket = TEXT("RightShoulderSocket");
	const FName LeftHand = TEXT("LeftHandSocket");
}

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
class DREAMSCAPE_API ADSBoss01 : public ADSBossCharacterBase, public IDSAttackTraceInterface, public IDSWeaponAttachInterface
{
	GENERATED_BODY()
public:
	ADSBoss01();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Boss|Action")
	TArray<FBossAction> Actions;

	UPROPERTY(EditAnywhere, Category = "Boss|Weapon")
	TObjectPtr<class UDSWeaponItemData> WeaponData;

	UPROPERTY(VisibleAnywhere, Category = "Animation")
	TObjectPtr<class UDSEnemyAnimInstance> AnimInstance;

	UPROPERTY()
	TObjectPtr<class ADSSwordWeapon> EquippedWeapon;

	const FBossAction* SelectAction();

	void ExecuteAction(const FBossAction& Action);

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual void OnDeath() override;

public:

	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;

	virtual void StartAttackTrace() override;
	virtual void PerformAttackTrace() override;
	virtual void EndAttackTrace() override;

	virtual void AttachWeaponBySocket(FName SocketName) override;

	UFUNCTION()
	void StartBossCombat();
};
