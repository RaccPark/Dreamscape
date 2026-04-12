// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/DSItemData.h"
#include "../DSSwordWeaponBase.h"
#include "DSWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSWeaponItemData : public UDSItemData
{
	GENERATED_BODY()
	
public:
	UDSWeaponItemData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Data")
	TSubclassOf<class ADSSwordWeaponBase> SwordWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Data")
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Data")
	float AttackRate;

	virtual void UseItem(class ACharacter* Character) override;
};
