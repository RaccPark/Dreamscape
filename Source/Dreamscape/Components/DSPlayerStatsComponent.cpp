// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DSPlayerStatsComponent.h"
#include "Character/Data/DSPlayerDataAsset.h"

UDSPlayerStatsComponent::UDSPlayerStatsComponent()
{
	// 틱 설정은 베이스(UDSCharacterStatsComponent)가 처리한다.
}

void UDSPlayerStatsComponent::InitializeStats()
{
	if (!PlayerData)
	{
		// 에셋이 지정되지 않아도 게임은 돌아야 한다.
		// CDO 기본값 = 기획서 수치이므로 임시 인스턴스를 만들어 쓴다.
		UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerStatsComponent] PlayerData가 지정되지 않았습니다. 기본값으로 초기화합니다."));
		PlayerData = NewObject<UDSPlayerDataAsset>(this);
	}

	Snapshot.MaxHealth = FMath::Clamp(PlayerData->DefaultMaxHealth, 1, PlayerData->HealthUpperLimit);
	Snapshot.MaxStamina = FMath::Clamp(PlayerData->DefaultMaxStamina, 1, PlayerData->StaminaUpperLimit);
	Snapshot.UpgradePoints.Empty();

	OnStatsChanged.Broadcast();
}

void UDSPlayerStatsComponent::ApplySnapshot(const FDSPlayerStatsSnapshot& NewSnapshot)
{
	Snapshot = NewSnapshot;

	OnStatsChanged.Broadcast();
}

int32 UDSPlayerStatsComponent::GetMaxHealth() const
{
	return Snapshot.MaxHealth;
}

int32 UDSPlayerStatsComponent::GetMaxStamina() const
{
	return Snapshot.MaxStamina;
}

float UDSPlayerStatsComponent::GetStaminaRegenPerSecond() const
{
	return PlayerData ? PlayerData->StaminaRegenPerSecond : 0.0f;
}

int32 UDSPlayerStatsComponent::GetUpgradePoint(EDSPlayerStatType StatType) const
{
	const int32* FoundPoint = Snapshot.UpgradePoints.Find(StatType);
	return FoundPoint ? *FoundPoint : 0;
}

float UDSPlayerStatsComponent::GetAttackPower() const
{
	if (!PlayerData)
	{
		return 0.0f;
	}

	return PlayerData->BaseAttackPower + (GetUpgradePoint(EDSPlayerStatType::AttackPower) * PlayerData->AttackPowerPerPoint);
}

float UDSPlayerStatsComponent::GetSubAttackPower() const
{
	if (!PlayerData)
	{
		return 0.0f;
	}

	return PlayerData->BaseSubAttackPower + (GetUpgradePoint(EDSPlayerStatType::SubAttackPower) * PlayerData->SubAttackPowerPerPoint);
}

float UDSPlayerStatsComponent::GetAttackSpeedMultiplier() const
{
	if (!PlayerData)
	{
		return 1.0f;
	}

	return 1.0f + (GetUpgradePoint(EDSPlayerStatType::AttackSpeed) * PlayerData->AttackSpeedBonusPerPoint);
}

float UDSPlayerStatsComponent::GetMoveSpeedMultiplier() const
{
	if (!PlayerData)
	{
		return 1.0f;
	}

	return 1.0f + (GetUpgradePoint(EDSPlayerStatType::Mobility) * PlayerData->MoveSpeedBonusPerPoint);
}

float UDSPlayerStatsComponent::GetAttackRangeMultiplier() const
{
	if (!PlayerData)
	{
		return 1.0f;
	}

	return 1.0f + (GetUpgradePoint(EDSPlayerStatType::AttackPower) * PlayerData->AttackRangeBonusPerPoint);
}

float UDSPlayerStatsComponent::GetRollCooldown() const
{
	if (!PlayerData)
	{
		return 0.0f;
	}

	const float Reduced = PlayerData->BaseRollCooldown - (GetUpgradePoint(EDSPlayerStatType::Mobility) * PlayerData->RollCooldownReductionPerPoint);
	return FMath::Max(Reduced, PlayerData->MinRollCooldown);
}

float UDSPlayerStatsComponent::GetAttackRecoveryTime(float WeaponRecoveryTime) const
{
	if (!PlayerData || PlayerData->AttackRecoveryRateMultiplier <= 0.0f)
	{
		return WeaponRecoveryTime;
	}

	return WeaponRecoveryTime / PlayerData->AttackRecoveryRateMultiplier;
}

float UDSPlayerStatsComponent::GetSubAttackChargeRateMultiplier() const
{
	
	if (!PlayerData || PlayerData->SubAttackChargeRateMultiplier <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerStatsComponent] PlayerData가 지정되지 않았거나 SubAttackChargeRateMultiplier가 0 이하입니다. 기본값 1.0을 반환합니다."));
		return 1.0f;
	}

	return PlayerData->SubAttackChargeRateMultiplier;
}

float UDSPlayerStatsComponent::GetSubAttackRecoveryTime() const
{
	return PlayerData ? PlayerData->SubAttackRecoveryTime : 0.0f;
}

bool UDSPlayerStatsComponent::AddUpgradePoint(EDSPlayerStatType StatType, int32 Amount)
{
	if (!PlayerData || Amount <= 0)
	{
		return false;
	}

	const int32 CurrentPoint = GetUpgradePoint(StatType);
	if (CurrentPoint >= PlayerData->MaxUpgradePointPerStat)
	{
		return false;
	}

	const int32 NewPoint = FMath::Clamp(CurrentPoint + Amount, 0, PlayerData->MaxUpgradePointPerStat);
	Snapshot.UpgradePoints.Add(StatType, NewPoint);

	OnStatsChanged.Broadcast();

	return true;
}

void UDSPlayerStatsComponent::AddMaxHealth(int32 Amount)
{
	if (!PlayerData || Amount == 0)
	{
		return;
	}

	Snapshot.MaxHealth = FMath::Clamp(Snapshot.MaxHealth + Amount, 1, PlayerData->HealthUpperLimit);

	OnStatsChanged.Broadcast();
}

void UDSPlayerStatsComponent::AddMaxStamina(int32 Amount)
{
	if (!PlayerData || Amount == 0)
	{
		return;
	}

	Snapshot.MaxStamina = FMath::Clamp(Snapshot.MaxStamina + Amount, 1, PlayerData->StaminaUpperLimit);

	OnStatsChanged.Broadcast();
}
