// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DSInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREAMSCAPE_API UDSInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDSInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	bool AddItem(class UDSItemData* ItemData, int32 Quantity);

	void AddCurrency(int32 Amount);
	bool SpendCurrency(int32 Amount);

	int32 GetCurrency() const;

	FOnInventoryUpdated OnInventoryUpdated;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// 무기 슬롯 (5)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Weapons")
	TArray<struct FInventorySlot> WeaponSlots;

	// 수집품 슬롯 (20)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Collectibles")
	TArray<struct FInventorySlot> CollectibleSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 MaxWeaponSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 MaxCollectibleSlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 CurrentCurrency;
};
