// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DSGameState.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCrystalDestroyed, int32, CurrentCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllCrystalsDestroyed);

UCLASS()
class DREAMSCAPE_API ADSGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ADSGameState();

	FOnCrystalDestroyed OnCrystalDestroyedDelegate;
	FOnAllCrystalsDestroyed OnAllCrystalsDestroyedDelegate;

private:
	int32 DestroyedCrystalCount;
	int32 TotalCrystalCount;

public:
	void OnCrystalDestroyed();
};
