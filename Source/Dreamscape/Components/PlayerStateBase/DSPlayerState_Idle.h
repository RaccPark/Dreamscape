// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerStateBase/DSPlayerStateBase.h"
#include "DSPlayerState_Idle.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSPlayerState_Idle : public UDSPlayerStateBase
{
	GENERATED_BODY()
	
public:
	virtual void OnRoll() override;
};
