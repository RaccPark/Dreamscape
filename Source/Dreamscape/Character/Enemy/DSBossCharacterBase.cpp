// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/DSBossCharacterBase.h"

void ADSBossCharacterBase::ApplyDamage(float DamageAmount)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0.0f)
	{
		OnDeath();
		return;
	}
}

void ADSBossCharacterBase::ApplyDamageWithKnockback(float DamageAmount, const FVector& KnockbackDirection, float KnockbackStrength)
{
	ApplyDamage(DamageAmount);
	// LaunchCharacter(KnockbackDirection * KnockbackStrength, true, true);
}
