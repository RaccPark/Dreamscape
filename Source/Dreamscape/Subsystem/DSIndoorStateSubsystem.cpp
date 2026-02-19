// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DSIndoorStateSubsystem.h"

FGameplayTag& UDSIndoorStateSubsystem::GetCurrentRoom()
{
	return CurrentRoomTag;
}

void UDSIndoorStateSubsystem::SetCurrentRoom(const FGameplayTag& NewRoomTag)
{
	if (!NewRoomTag.IsValid())
	{
		return;
	}

	// 동일 Room이면 무시
	if (CurrentRoomTag == NewRoomTag)
	{
		return;
	}

	CurrentRoomTag = NewRoomTag;

	OnRoomChanged.Broadcast(CurrentRoomTag);
}
