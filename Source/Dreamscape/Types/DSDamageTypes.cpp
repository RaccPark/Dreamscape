// Fill out your copyright notice in the Description page of Project Settings.


#include "Types/DSDamageTypes.h"
#include "GameFramework/Actor.h"

void FDSDamageInfo::SetKnockback(const FVector& Direction, float Strength)
{
	// 받는 쪽은 Direction * Strength를 그대로 LaunchCharacter에 넘긴다.
	// 길이가 1이 아니면 거리만큼 힘이 커지고, Z가 남으면 땅으로 박히거나 뜬다.
	KnockbackDirection = FVector(Direction.X, Direction.Y, 0.0f).GetSafeNormal();
	KnockbackStrength = FMath::Max(0.0f, Strength);
}

void FDSDamageInfo::SetKnockbackFrom(const FVector& FromLocation, const AActor* Target, float Strength)
{
	if (!Target)
	{
		return;
	}

	SetKnockback(Target->GetActorLocation() - FromLocation, Strength);
}
