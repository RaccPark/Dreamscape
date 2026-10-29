// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Data/DSPlayerDataAsset.h"

UDSPlayerDataAsset::UDSPlayerDataAsset()
{
	// 자원 — 기획서 "기본 플레이어 스탯"
	DefaultMaxHealth = 4;
	HealthUpperLimit = 6;
	DefaultMaxStamina = 4;
	StaminaUpperLimit = 8;
	StaminaRegenPerSecond = 1.0f;	// 기획 미정. 잠정값

	// 전투
	BaseAttackPower = 1.0f;
	BaseSubAttackPower = 2.0f;
	AttackRecoveryRateMultiplier = 1.0f;
	SubAttackChargeRateMultiplier = 1.0f;
	SubAttackRecoveryTime = 0.25f;

	// 이동
	BaseRollCooldown = 0.5f;
	MinRollCooldown = 0.25f;

	// 업그레이드 — 전부 합연산(%p)
	MaxUpgradePointPerStat = 5;
	AttackPowerPerPoint = 0.1f;
	AttackRangeBonusPerPoint = 0.02f;
	SubAttackPowerPerPoint = 0.1f;
	AttackSpeedBonusPerPoint = 0.05f;
	MoveSpeedBonusPerPoint = 0.05f;
	RollCooldownReductionPerPoint = 0.05f;
}
