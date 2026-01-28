// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM()
enum class EDSPlaceType : uint8
{
	EPT_Outdoor UMETA(DisplayName = "Outdoor"),
	EPT_Indoor UMETA(DisplayName = "Indoor"),
};

class DREAMSCAPE_API DSPlaceType
{
public:
	DSPlaceType();
	~DSPlaceType();
};
