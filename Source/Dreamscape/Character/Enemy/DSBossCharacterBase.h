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
	// 보스는 넉백되지 않는다. 넉백을 지운 사본으로 베이스를 부른다.
	virtual void ApplyDamage(const FDSDamageInfo& DamageInfo) override;

	FOnBossLandingFinished OnBossLandingFinishedDelegate;
};
