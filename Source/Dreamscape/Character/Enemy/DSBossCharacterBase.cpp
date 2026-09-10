// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/DSBossCharacterBase.h"

void ADSBossCharacterBase::ApplyDamageWithKnockback(float DamageAmount, const FVector& KnockbackDirection, float KnockbackStrength)
{
	// 보스는 넉백되지 않는다. 베이스와 달리 LaunchCharacter를 부르지 않는다.
	ApplyDamage(DamageAmount);
}
