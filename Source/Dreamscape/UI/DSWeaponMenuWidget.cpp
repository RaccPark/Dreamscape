// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DSWeaponMenuWidget.h"
#include "Components/GridPanel.h"
#include "DSWeaponSlotWidget.h"
#include "Components/Button.h"

#include "Character/DSCharacterPlayer.h"
#include "Components/DSInventoryComponent.h"
#include "Components/Inventory/DSInventorySlot.h"


void UDSWeaponMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponSlotWidgets.Empty();

	if (!Weapon_Inventory_GridPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon_Inventory_GridPanel is not bound in UDSWeaponMenuWidget"));
		return;
	}

	const int32 ChildCount = Weapon_Inventory_GridPanel->GetChildrenCount();
	for (int32 i = 0; i < ChildCount; ++i)
	{
		UDSWeaponSlotWidget* WeaponSlotWidget = Cast<UDSWeaponSlotWidget>(Weapon_Inventory_GridPanel->GetChildAt(i));
		if (WeaponSlotWidget)
		{
			WeaponSlotWidgets.Add(WeaponSlotWidget);

			WeaponSlotWidget->SetSlotIndex(i);

			WeaponSlotWidget->OnSlotClickedDelegate.Clear();
			WeaponSlotWidget->OnSlotClickedDelegate.AddDynamic(this, &UDSWeaponMenuWidget::OnWeaponSlotClicked);
		}
	}

	ADSCharacterPlayer* Player = GetOwningPlayerPawn<ADSCharacterPlayer>();
	if (Player)
	{
		UDSInventoryComponent* InventoryComponent = Player->GetInventoryComponent();
		if (InventoryComponent)
		{
			InventoryComponent->OnWeaponEquippedDelegate.Clear();
			InventoryComponent->OnWeaponEquippedDelegate.AddDynamic(this, &UDSWeaponMenuWidget::UpdateEquipUI);

			InventoryComponent->OnInventoryUpdated.Clear();
			InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UDSWeaponMenuWidget::RefreshSlots);
		}
	}

	RefreshSlots();
}

void UDSWeaponMenuWidget::OnWeaponSlotClicked(int32 SlotIndex)
{
	UE_LOG(LogTemp, Log, TEXT("Weapon Slot %d clicked in Weapon Menu"), SlotIndex);

	if (SelectedSlotIndex == SlotIndex)
	{
		UE_LOG(LogTemp, Log, TEXT("Weapon Slot %d is already selected"), SlotIndex);
		return;
	}

	SelectedSlotIndex = SlotIndex;
	ADSCharacterPlayer* Player = GetOwningPlayerPawn<ADSCharacterPlayer>();
	if (Player)
	{
		UDSInventoryComponent* InventoryComponent = Player->GetInventoryComponent();
		if (InventoryComponent)
		{
			InventoryComponent->EquipWeapon(SlotIndex);
		}
	}
}

void UDSWeaponMenuWidget::UpdateEquipUI(int32 SlotIndex, UDSItemData* ItemData)
{
	UE_LOG(LogTemp, Log, TEXT("Updating Equip UI for Slot %d"), SlotIndex);
}

void UDSWeaponMenuWidget::RefreshSlots()
{
	ADSCharacterPlayer* Player = GetOwningPlayerPawn<ADSCharacterPlayer>();
	if (!Player)
	{
		return;
	}

	UDSInventoryComponent* InventoryComponent = Player->GetInventoryComponent();
	if (!InventoryComponent)
	{
		return;
	}

	TArray<FInventorySlot>& WeaponSlots = InventoryComponent->GetWeaponSlots();
	if (!WeaponSlots.Num())
	{
		return;
	}
			
	for (int32 i = 0; i < WeaponSlotWidgets.Num(); ++i)
	{
		if (WeaponSlots.IsValidIndex(i))
		{
			const FInventorySlot& aSlot = WeaponSlots[i];
			WeaponSlotWidgets[i]->SetItemData(aSlot.ItemData);
		}
		else
		{
			WeaponSlotWidgets[i]->SetItemData(nullptr);
		}
	}
}
