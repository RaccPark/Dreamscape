// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "DSIndoorStateSubsystem.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRoomChangedDelegate, FGameplayTag);

UCLASS()
class DREAMSCAPE_API UDSIndoorStateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	FGameplayTag& GetCurrentRoom();
	void SetCurrentRoom(const FGameplayTag& NewRoomTag);

	FOnRoomChangedDelegate OnRoomChanged;

private:
	FGameplayTag CurrentRoomTag;
};
