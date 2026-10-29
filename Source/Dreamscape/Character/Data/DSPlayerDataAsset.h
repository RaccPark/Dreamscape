// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DSPlayerDataAsset.generated.h"

/**
 * 플레이어의 기본 스탯 수치를 담는 데이터 에셋.
 * UDSPlayerStatsComponent가 이 값으로 초기화된다.
 *
 * 주의: UE는 CDO와 같은 값을 .uasset에 직렬화하지 않는다.
 * 따라서 에셋에서 "건드리지 않은" 값은 아래 생성자 기본값을 그대로 따라간다.
 * 생성자 기본값을 기획서 수치와 일치시켜 둘 것.
 */
UCLASS()
class DREAMSCAPE_API UDSPlayerDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UDSPlayerDataAsset();

// ==================================================
// 자원 (체력 / 스테미나)
// ==================================================
public:
	// 시작 최대 체력 (기획: 4)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Vitality", Meta = (ClampMin = "1"))
	int32 DefaultMaxHealth;

	// 최대 체력의 상한 (기획: 6)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Vitality", Meta = (ClampMin = "1"))
	int32 HealthUpperLimit;

	// 시작 최대 스테미나 (기획: 4)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Vitality", Meta = (ClampMin = "1"))
	int32 DefaultMaxStamina;

	// 최대 스테미나의 상한 (기획: 8)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Vitality", Meta = (ClampMin = "1"))
	int32 StaminaUpperLimit;

	// 초당 스테미나 자동 회복량.
	// 기획서에 회복 규칙이 없어 잠정으로 둔 값이다. 규칙이 확정되면 여기만 바꾸면 된다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Vitality", Meta = (ClampMin = "0.0"))
	float StaminaRegenPerSecond;

// ==================================================
// 전투 기본 스탯
// ==================================================
public:
	// 기본 공격력 (기획: 1)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Combat", Meta = (ClampMin = "0.0"))
	float BaseAttackPower;

	// 보조 공격력 (기획: 2)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Combat", Meta = (ClampMin = "0.0"))
	float BaseSubAttackPower;

	// 기본 공격 회복 속도 배수 (기획: 1.0x).
	// 최종 회복시간 = 무기 회복시간 / 이 배수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Combat", Meta = (ClampMin = "0.01"))
	float AttackRecoveryRateMultiplier;

	// 보조 공격 충전 속도 배수 (기획: 1.0x)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Combat", Meta = (ClampMin = "0.01"))
	float SubAttackChargeRateMultiplier;

	// 보조 공격 회복 시간 (기획: 0.25초)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Combat", Meta = (ClampMin = "0.0"))
	float SubAttackRecoveryTime;

// ==================================================
// 이동
// ==================================================
public:
	// 구르기(회피) 쿨타임 (기획: 0.5초)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Mobility", Meta = (ClampMin = "0.0"))
	float BaseRollCooldown;

	// 구르기 쿨타임 하한 (기획: 0.25초)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Mobility", Meta = (ClampMin = "0.0"))
	float MinRollCooldown;

// ==================================================
// 업그레이드 (포인트당 증가량)
// ==================================================
public:
	// 축별 최대 포인트 (기획: 5)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Upgrade", Meta = (ClampMin = "0"))
	int32 MaxUpgradePointPerStat;

	// 기본 공격력 포인트당 +0.1
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Upgrade")
	float AttackPowerPerPoint;

	// 사거리 포인트당 +2%p (합연산: 배수 = 1.0 + 0.02 * 포인트)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Upgrade")
	float AttackRangeBonusPerPoint;

	// 보조 공격력 포인트당 +0.1
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Upgrade")
	float SubAttackPowerPerPoint;

	// 공격 속도(애니메이션 배속) 포인트당 +5%p
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Upgrade")
	float AttackSpeedBonusPerPoint;

	// 이동 속도 포인트당 +5%p
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Upgrade")
	float MoveSpeedBonusPerPoint;

	// 구르기 쿨타임 포인트당 -0.05초
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Upgrade")
	float RollCooldownReductionPerPoint;
};
