// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DSWeaponSlotWidget.h"
#include "Components/Image.h"
#include "Components/Inventory/DSItemData.h"

void UDSWeaponSlotWidget::NativeConstruct()
{
}

FReply UDSWeaponSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!CurrentItemData)
	{
		return FReply::Unhandled();
	}

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		UE_LOG(LogTemp, Log, TEXT("Weapon Slot %d clicked"), SlotIndex);

		if (OnSlotClickedDelegate.IsBound())
		{
			OnSlotClickedDelegate.Broadcast(SlotIndex);
		}

		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UDSWeaponSlotWidget::SetItemData(UDSItemData* ItemData)
{
	CurrentItemData = ItemData;

	if (!CurrentItemData)
	{
		if (WeaponSlot_Weapon_Image)
		{
			WeaponSlot_Weapon_Image->SetVisibility(ESlateVisibility::Hidden);
		}
		return;
	}
	
	if (WeaponSlot_Weapon_Image && CurrentItemData->ItemIcon)
	{
		WeaponSlot_Weapon_Image->SetBrushFromTexture(CurrentItemData->ItemIcon);
		WeaponSlot_Weapon_Image->SetVisibility(ESlateVisibility::Visible);
	}
}

void UDSWeaponSlotWidget::SetSlotIndex(int32 NewSlotIndex)
{
	SlotIndex = NewSlotIndex;
}

int32 UDSWeaponSlotWidget::GetSlotIndex() const
{
	return SlotIndex;
}
