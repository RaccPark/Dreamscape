// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerStateBase/DSPlayerStateBase.h"
#include "DSPlayerState_Land.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSPlayerState_Land : public UDSPlayerStateBase
{
	GENERATED_BODY()
	
public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;


};
