// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DSWeaponMenuWidget.generated.h"

/**
 * 
 */

UCLASS()
class DREAMSCAPE_API UDSWeaponMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UGridPanel> Weapon_Inventory_GridPanel;

	TArray<TObjectPtr<class UDSWeaponSlotWidget>> WeaponSlotWidgets;

	int32 SelectedSlotIndex;

protected:
	UFUNCTION()
	void OnWeaponSlotClicked(int32 SlotIndex);

	UFUNCTION()
	void UpdateEquipUI(int32 SlotIndex, class UDSItemData* ItemData);

	UFUNCTION()
	void RefreshSlots();
};
