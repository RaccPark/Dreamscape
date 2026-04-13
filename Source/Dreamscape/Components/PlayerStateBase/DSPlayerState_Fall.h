// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerStateBase/DSPlayerStateBase.h"
#include "DSPlayerState_Fall.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSPlayerState_Fall : public UDSPlayerStateBase
{
	GENERATED_BODY()
	
public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;

	virtual void OnSwordAttack() override;

	virtual void OnDeath() override;
};
