// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DSItemData.generated.h"

UENUM()
enum class EItemType : uint8
{
	IT_Weapon	UMETA(DisplayName = "Weapon"),
	IT_Collectible	UMETA(DisplayName = "Collectible"),
	IT_Consumable	UMETA(DisplayName = "Consumable"),
};

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemDescription;

	// 아이템 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<class UTexture2D> ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	EItemType ItemType;
	
	// 무기와 수집품은 1, 그 외 소모품은 99까지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 FixedSlotIndex;

	virtual void UseItem(class ACharacter* Character) PURE_VIRTUAL(UDSItemData::UseItem, );
};
