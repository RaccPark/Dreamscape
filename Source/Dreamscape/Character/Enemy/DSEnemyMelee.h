// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/DSEnemyCharacterBase.h"
#include "Interface/DSDamageableInterface.h"
#include "DSEnemyMelee.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API ADSEnemyMelee : public ADSEnemyCharacterBase, public IDSDamageableInterface
{
	GENERATED_BODY()
	
public:
	ADSEnemyMelee();
	
protected:
	virtual void BeginPlay() override;

	virtual void OnDeath() override;

public:
	virtual void ApplyDamage(float DamageAmount) override;
	virtual void ApplyDamageWithKnockback(float DamageAmount, const FVector& KnockbackDirection, float KnockbackStrength) override;

	virtual void Attack() override;

	void UpdateWalkSpeed(float NewSpeed);
};
