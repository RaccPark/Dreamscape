// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSDamageTypes.generated.h"

/**
 * 기본 공격, 보조 공격, 폭발, 적 공격 등
 * 공격 종류를 분류하기 위한 enum
 * 공격 종류를 분류하여 폭발 경직 등의 효과를 적용할 수 있음.
 */
UENUM(BlueprintType)
enum class EDSDamageType : uint8
{
	MainAttack UMETA(DisplayName = "Main Attack"),
	SubAttack UMETA(DisplayName = "Sub Attack"),
	Explosion UMETA(DisplayName = "Explosion"),
	EnemyAttack UMETA(DisplayName = "Enemy Attack"),
};
