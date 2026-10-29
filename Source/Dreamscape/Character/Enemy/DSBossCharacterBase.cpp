// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/DSBossCharacterBase.h"

void ADSBossCharacterBase::ApplyDamage(const FDSDamageInfo& DamageInfo)
{
	// 보스는 넉백되지 않는다. 세기를 0으로 지우면 베이스의 HasKnockback()이 false가 되어 LaunchCharacter를 건너뛴다.
	FDSDamageInfo NoKnockbackInfo = DamageInfo;
	NoKnockbackInfo.KnockbackStrength = 0.0f;

	Super::ApplyDamage(NoKnockbackInfo);
}
