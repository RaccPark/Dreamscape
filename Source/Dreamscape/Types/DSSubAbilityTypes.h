// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSSubAbilityTypes.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EDSSubAbilityType : uint8
{
	Arrow UMETA(DisplayName = "Arrow"),
	Cone UMETA(DisplayName = "Cone"),
	Explosion UMETA(DisplayName = "Explosion"),
	Dash UMETA(DisplayName = "Dash"),
};
