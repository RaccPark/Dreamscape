// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DSVitalityComponent.h"
#include "Components/DSCharacterStatsComponent.h"

UDSVitalityComponent::UDSVitalityComponent()
{
	// 공통 체력 컴포넌트는 매 프레임 할 일이 없다.
	// 스테미나 자동 회복이 필요한 UDSPlayerVitalityComponent가 다시 켠다.
	PrimaryComponentTick.bCanEverTick = false;

	CurrentHealth = 0.0f;
	CachedMaxHealth = 0.0f;
	bIsInvincible = false;
	bIsDead = false;
}

void UDSVitalityComponent::Initialize(UDSCharacterStatsComponent* InStatsComponent)
{
	if (!InStatsComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UDSVitalityComponent] StatsComponent가 nullptr입니다."));
		return;
	}

	StatsComponent = InStatsComponent;
	StatsComponent->OnStatsChanged.AddDynamic(this, &UDSVitalityComponent::RefreshMaxValues);

	CachedMaxHealth = static_cast<float>(StatsComponent->GetMaxHealth());
	CurrentHealth = CachedMaxHealth;

	bIsDead = false;
	bIsInvincible = false;

	OnHealthChanged.Broadcast(CurrentHealth, CachedMaxHealth);
}

void UDSVitalityComponent::RefreshMaxValues()
{
	if (!StatsComponent)
	{
		return;
	}

	CachedMaxHealth = static_cast<float>(StatsComponent->GetMaxHealth());
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, CachedMaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth, CachedMaxHealth);
}

float UDSVitalityComponent::SanitizeDamage(float RawDamage) const
{
	// 적 캐릭터는 무기 배수가 만든 소수 피해를 그대로 받는다.
	return RawDamage;
}

bool UDSVitalityComponent::ApplyDamage(float DamageAmount, EDSDamageType DamageType, AActor* DamageInstigator)
{
	if (bIsDead || bIsInvincible || DamageAmount <= 0.0f)
	{
		return false;
	}

	const float FinalDamage = SanitizeDamage(DamageAmount);
	if (FinalDamage <= 0.0f)
	{
		return false;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - FinalDamage, 0.0f, CachedMaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth, CachedMaxHealth);
	OnDamaged.Broadcast(FinalDamage, DamageType, DamageInstigator);

	UE_LOG(LogTemp, Log, TEXT("[UDSVitalityComponent] %s 피해 %.2f -> 체력 %.2f / %.2f"),
		*GetNameSafe(GetOwner()), FinalDamage, CurrentHealth, CachedMaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		bIsDead = true;
		OnDeath.Broadcast();
	}
	else
	{
		OnStaggered.Broadcast();
	}

	return true;
}

void UDSVitalityComponent::Heal(float Amount)
{
	if (bIsDead || Amount <= 0.0f)
	{
		return;
	}

	const float FinalAmount = SanitizeDamage(Amount);
	const float PreviousHealth = CurrentHealth;

	CurrentHealth = FMath::Clamp(CurrentHealth + FinalAmount, 0.0f, CachedMaxHealth);

	if (!FMath::IsNearlyEqual(PreviousHealth, CurrentHealth))
	{
		OnHealthChanged.Broadcast(CurrentHealth, CachedMaxHealth);
	}
}

void UDSVitalityComponent::SetInvincible(bool bNewInvincible)
{
	bIsInvincible = bNewInvincible;
}
