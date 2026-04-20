// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DSWeaponSlotWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, int32, SlotIndex);

UCLASS()
class DREAMSCAPE_API UDSWeaponSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> WeaponSlot_Background_Image;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> WeaponSlot_Weapon_Image;

	UDSItemData* CurrentItemData;

	int32 SlotIndex;

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	FOnSlotClicked OnSlotClickedDelegate;

	UFUNCTION()
	void SetItemData(class UDSItemData* ItemData);

	void SetSlotIndex(int32 NewSlotIndex);
	int32 GetSlotIndex() const;
};
