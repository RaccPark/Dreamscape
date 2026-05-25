// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EDSBossEncounterState : uint8
{
    BES_Idle,
    BES_DoorClosing,
    BES_BossSpawning,
    BES_WaitingForLanding,
    BES_InCombat,
    BES_BossDead,
    BES_Finished
};
