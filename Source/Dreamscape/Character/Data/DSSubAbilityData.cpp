// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Data/DSSubAbilityData.h"

UDSSubAbilityData::UDSSubAbilityData()
{
	// 초기 Arrow 능력으로 초기화
	// 이후 값을 각 능력에 맞게 덮어 씌움
	SubAbilityType = EDSSubAbilityType::Arrow;
	DamageMultiplier = 1.0f;
	StaminaCost = 1.0f;
	ChargeTime = 0.25f;
	ProjectileSpeed = 2000.0f;	// 임시 값
}
