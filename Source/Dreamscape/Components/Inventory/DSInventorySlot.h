// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/DSItemData.h"
#include "DSInventorySlot.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	UDSItemData* ItemData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Quantity;

	FInventorySlot() : ItemData(nullptr), Quantity(0)
	{
	}

	FInventorySlot(UDSItemData* InItemData, int32 InQuantity) : ItemData(InItemData), Quantity(InQuantity)
	{
	}

	bool IsEmpty() const
	{
		return ItemData == nullptr || Quantity <= 0;
	}
};
