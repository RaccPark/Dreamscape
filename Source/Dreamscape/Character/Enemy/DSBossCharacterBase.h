// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/DSEnemyCharacterBase.h"
#include "DSBossCharacterBase.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossLandingFinished);

UCLASS()
class DREAMSCAPE_API ADSBossCharacterBase : public ADSEnemyCharacterBase
{
	GENERATED_BODY()

public:
	// 보스는 넉백되지 않는다. 피해만 적용하고 LaunchCharacter는 건너뛴다.
	virtual void ApplyDamageWithKnockback(float DamageAmount, const FVector& KnockbackDirection, float KnockbackStrength) override;

	FOnBossLandingFinished OnBossLandingFinishedDelegate;
};
