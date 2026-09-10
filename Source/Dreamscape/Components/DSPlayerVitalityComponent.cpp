// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DSPlayerVitalityComponent.h"
#include "Components/DSPlayerStatsComponent.h"

UDSPlayerVitalityComponent::UDSPlayerVitalityComponent()
{
	// 스테미나 자동 회복 때문에 틱이 필요하다. 베이스는 꺼 두었다.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentStamina = 0.0f;
	CachedMaxStamina = 0.0f;
}

void UDSPlayerVitalityComponent::Initialize(UDSCharacterStatsComponent* InStatsComponent)
{
	Super::Initialize(InStatsComponent);

	PlayerStatsComponent = Cast<UDSPlayerStatsComponent>(InStatsComponent);
	if (!PlayerStatsComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerVitalityComponent] StatsComponent가 UDSPlayerStatsComponent가 아닙니다. 스테미나가 동작하지 않습니다."));
		return;
	}

	CachedMaxStamina = static_cast<float>(PlayerStatsComponent->GetMaxStamina());
	CurrentStamina = CachedMaxStamina;

	OnStaminaChanged.Broadcast(CurrentStamina, CachedMaxStamina);
}

void UDSPlayerVitalityComponent::RefreshMaxValues()
{
	Super::RefreshMaxValues();

	if (!PlayerStatsComponent)
	{
		return;
	}

	CachedMaxStamina = static_cast<float>(PlayerStatsComponent->GetMaxStamina());
	CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, CachedMaxStamina);

	OnStaminaChanged.Broadcast(CurrentStamina, CachedMaxStamina);
}

void UDSPlayerVitalityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsDead() || !PlayerStatsComponent)
	{
		return;
	}

	// 스테미나 자동 회복
	if (CurrentStamina < CachedMaxStamina)
	{
		RestoreStamina(PlayerStatsComponent->GetStaminaRegenPerSecond() * DeltaTime);
	}
}

float UDSPlayerVitalityComponent::SanitizeDamage(float RawDamage) const
{
	return FMath::Max(1.0f, FMath::RoundToFloat(RawDamage));
}

bool UDSPlayerVitalityComponent::CanConsumeStamina(float Amount) const
{
	return !IsDead() && CurrentStamina >= Amount;
}

bool UDSPlayerVitalityComponent::ConsumeStamina(float Amount)
{
	if (Amount <= 0.0f || !CanConsumeStamina(Amount))
	{
		return false;
	}

	CurrentStamina = FMath::Clamp(CurrentStamina - Amount, 0.0f, CachedMaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, CachedMaxStamina);

	return true;
}

void UDSPlayerVitalityComponent::RestoreStamina(float Amount)
{
	if (IsDead() || Amount <= 0.0f)
	{
		return;
	}

	const float PreviousStamina = CurrentStamina;

	CurrentStamina = FMath::Clamp(CurrentStamina + Amount, 0.0f, CachedMaxStamina);

	if (!FMath::IsNearlyEqual(PreviousStamina, CurrentStamina))
	{
		OnStaminaChanged.Broadcast(CurrentStamina, CachedMaxStamina);
	}
}
