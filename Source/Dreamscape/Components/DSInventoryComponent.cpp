// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DSInventoryComponent.h"
#include "Components/Inventory/DSItemData.h"
#include "Components/Inventory/DSInventorySlot.h"

#include "Character/DSCharacterPlayer.h"

// Sets default values for this component's properties
UDSInventoryComponent::UDSInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxWeaponSlots = 5;
	MaxCollectibleSlots = 12;
	CurrentCurrency = 0;
	CurrentWeaponSlotIndex = 0;
}


// Called when the game starts
void UDSInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// 고정 크기만큼 슬롯 배열 초기화
	WeaponSlots.SetNum(MaxWeaponSlots);
	CollectibleSlots.SetNum(MaxCollectibleSlots);
}

bool UDSInventoryComponent::AddItem(UDSItemData* ItemData, int32 Quantity)
{
	if (!ItemData || Quantity <= 0)
	{
		return false;
	}

	int32 TargetIndex = ItemData->FixedSlotIndex;

	if (ItemData->ItemType == EItemType::IT_Weapon)
	{
		if (WeaponSlots.IsValidIndex(TargetIndex))
		{
			// 중복 획득 시 무시 처리
			if (WeaponSlots[TargetIndex].IsEmpty())
			{
				WeaponSlots[TargetIndex] = FInventorySlot(ItemData, 1);
				OnInventoryUpdated.Broadcast();
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Weapon slot %d is already occupied."), TargetIndex);
				return false;
			}
		}
	}
	else if (ItemData->ItemType == EItemType::IT_Collectible)
	{
		if (CollectibleSlots.IsValidIndex(TargetIndex))
		{
			// 중복 획득 시 무시 처리
			if (CollectibleSlots[TargetIndex].IsEmpty())
			{
				CollectibleSlots[TargetIndex] = FInventorySlot(ItemData, 1);
				OnInventoryUpdated.Broadcast();
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Collectible slot %d is already occupied."), TargetIndex);
				return false;
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Invalid item type or slot index for item %s."), *ItemData->ItemName.ToString());
	return false;
}


void UDSInventoryComponent::AddCurrency(int32 Amount)
{
	if (Amount > 0)
	{
		CurrentCurrency += Amount;
		OnInventoryUpdated.Broadcast();
	}
}

bool UDSInventoryComponent::SpendCurrency(int32 Amount)
{
	if (Amount > 0 && CurrentCurrency >= Amount)
	{
		CurrentCurrency -= Amount;
		OnInventoryUpdated.Broadcast();
		return true;
	}

	return false;
}

int32 UDSInventoryComponent::GetCurrency() const
{
	return CurrentCurrency;
}

void UDSInventoryComponent::EquipWeapon(int32 SlotIndex)
{
	if (!WeaponSlots.IsValidIndex(SlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid weapon slot index: %d"), SlotIndex);
		return;
	}

	const FInventorySlot& TargetSlot = WeaponSlots[SlotIndex];

	if (TargetSlot.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon slot %d is empty."), SlotIndex);
		return;
	}

	CurrentWeapon = TargetSlot.ItemData;
	CurrentWeaponSlotIndex = SlotIndex;

	if (CurrentWeapon)
	{
		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
		if (!OwnerCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Owner is not a character!"));
			return;
		}
		CurrentWeapon->UseItem(OwnerCharacter);
	}
	
}

TArray<FInventorySlot>& UDSInventoryComponent::GetWeaponSlots()
{
	return WeaponSlots;
}


// Called every frame
void UDSInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

