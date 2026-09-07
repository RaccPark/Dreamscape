// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DSGameState.h"

ADSGameState::ADSGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	DestroyedCrystalCount = 0;
	TotalCrystalCount = 3;
}

void ADSGameState::OnCrystalDestroyed()
{
	DestroyedCrystalCount++;

	UE_LOG(LogTemp, Warning, TEXT("[ADSGameState] Crystal Destroyed: %d"), DestroyedCrystalCount);

	// UI Update
	OnCrystalDestroyedDelegate.Broadcast(DestroyedCrystalCount);

	if (DestroyedCrystalCount >= TotalCrystalCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ADSGameState] All Crystals Destroyed!"));

		OnAllCrystalsDestroyedDelegate.Broadcast();
	}
}
