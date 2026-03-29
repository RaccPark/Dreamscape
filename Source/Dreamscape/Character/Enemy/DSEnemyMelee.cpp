// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/DSEnemyMelee.h"

ADSEnemyMelee::ADSEnemyMelee()
{
	AttackRange = 10.0f;
	AttackDamage = 1.0f;
}

void ADSEnemyMelee::ApplyDamage(float DamageAmount)
{
	if (HitReactMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Playing hit react montage!"));
		PlayAnimMontage(HitReactMontage);
	}

	TakeDamage(DamageAmount);
}

void ADSEnemyMelee::ApplyDamageWithKnockback(float DamageAmount, const FVector& KnockbackDirection, float KnockbackStrength)
{
	ApplyDamage(DamageAmount);
	LaunchCharacter(KnockbackDirection * KnockbackStrength, true, true);
}
