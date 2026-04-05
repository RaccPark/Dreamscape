// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSEnemyState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EDSEnemyState : uint8
{
	ES_Idle		UMETA(DisplayName = "Idle"),
	ES_Patrol	UMETA(DisplayName = "Patrol"),
	ES_Chase	UMETA(DisplayName = "Chase"),
	ES_Attack	UMETA(DisplayName = "Attack"),
	ES_Dead 	UMETA(DisplayName = "Dead"),
}; 


