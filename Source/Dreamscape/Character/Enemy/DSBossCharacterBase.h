// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/DSEnemyCharacterBase.h"
#include "Interface/DSDamageableInterface.h"
#include "DSBossCharacterBase.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossLandingFinished);

UCLASS()
class DREAMSCAPE_API ADSBossCharacterBase : public ADSEnemyCharacterBase, public IDSDamageableInterface
{
	GENERATED_BODY()
	
public:
	virtual void ApplyDamage(float DamageAmount) override;
	virtual void ApplyDamageWithKnockback(float DamageAmount, const FVector& KnockbackDirection, float KnockbackStrength) override;

	FOnBossLandingFinished OnBossLandingFinishedDelegate;
};
