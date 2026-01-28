// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Enum/DSPlaceType.h"
#include "DSIndoorStateSubsystem.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAreaStateChangedDelegate, EDSPlaceType);

UCLASS()
class DREAMSCAPE_API UDSIndoorStateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	EDSPlaceType GetAreaState() const;
	void SetAreaState(EDSPlaceType NewState);

	FOnAreaStateChangedDelegate OnAreaStateChanged;

private:
	EDSPlaceType CurrentAreaState = EDSPlaceType::EPT_Outdoor;
};
