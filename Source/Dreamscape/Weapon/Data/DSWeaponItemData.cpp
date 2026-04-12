// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Data/DSWeaponItemData.h"
#include "Character/DSCharacterPlayer.h"

UDSWeaponItemData::UDSWeaponItemData()
{
	ItemType = EItemType::IT_Weapon;
	MaxStackSize = 1;
}

void UDSWeaponItemData::UseItem(ACharacter* Character)
{
	if (!Character || !SwordWeaponClass)
	{
		return;
	}

	ADSCharacterPlayer* PlayerCharacter = Cast<ADSCharacterPlayer>(Character);
	if (PlayerCharacter)
	{
		PlayerCharacter->EquipSwordWeapon(this);
	}
}
