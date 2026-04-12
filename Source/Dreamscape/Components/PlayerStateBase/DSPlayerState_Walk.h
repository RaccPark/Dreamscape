// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerStateBase/DSPlayerStateBase.h"
#include "DSPlayerState_Walk.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSPlayerState_Walk : public UDSPlayerStateBase
{
	GENERATED_BODY()
	
public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;

	virtual void OnMove(const struct FInputActionValue& Value) override;
	virtual void OnRoll() override;
	virtual void OnSwordAttack() override;
	
};
