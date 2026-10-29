// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Data/DSWeaponItemData.h"
#include "Character/DSCharacterPlayer.h"
#include "Components/DSPlayerCombatComponent.h"

UDSWeaponItemData::UDSWeaponItemData()
{
	ItemType = EItemType::IT_Weapon;
	MaxStackSize = 1;

	// 기본 검 기준값. 나머지 무기는 에셋에서 덮어쓴다.
	DamageMultiplier = 1.0f;
	DamageAmount = 1.0f;
	AttackRate = 1.0f;
	RecoveryTime = 0.5f;
	AttackRange = 100.0f;	// 테스트 값
	LungeImpulse = 600.0f;
}

void UDSWeaponItemData::UseItem(ACharacter* Character)
{
	if (!Character || !SwordWeaponClass)
	{
		return;
	}

	ADSCharacterPlayer* PlayerCharacter = Cast<ADSCharacterPlayer>(Character);
	if (!PlayerCharacter)
	{
		return;
	}

	if (UDSPlayerCombatComponent* CombatComponent = PlayerCharacter->GetCombatComponent())
	{
		CombatComponent->EquipWeapon(this);
	}
}
