// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSPlayerStatTypes.generated.h"

/**
 * 플레이어 스탯 업그레이드 축.
 * 기획서 "플레이어 세부 스탯 업그레이드 시스템"의 4종에 대응한다.
 * 각 축은 0~5 포인트를 가지며, 포인트당 증가량은 UDSPlayerDataAsset이 들고 있다.
 */
UENUM(BlueprintType)
enum class EDSPlayerStatType : uint8
{
	AttackPower		UMETA(DisplayName = "기본 공격력/사거리"),
	SubAttackPower	UMETA(DisplayName = "보조 공격력"),
	AttackSpeed		UMETA(DisplayName = "공격 속도"),
	Mobility		UMETA(DisplayName = "이동 속도/구르기"),
};

/**
 * 레벨 전환·세이브를 넘어 유지되어야 하는 플레이어 스탯.
 * 시스템 기획의 "메인 레벨 전환 간 처리 요소 - 플레이어 스탯/인벤토리: 유지" 요구사항 때문에
 * 처음부터 직렬화 가능한 단일 구조체로 들고 다닌다.
 */
USTRUCT(BlueprintType)
struct FDSPlayerStatsSnapshot
{
	GENERATED_BODY()

	// 현재 해금된 최대 체력 (기획 기본 4, 상한 6)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 MaxHealth = 4;

	// 현재 해금된 최대 스테미나 (기획 기본 4, 상한 8)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 MaxStamina = 4;

	// 업그레이드 축별 보유 포인트. 없는 키는 0으로 취급한다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	TMap<EDSPlayerStatType, int32> UpgradePoints;
};
