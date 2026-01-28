// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DSIndoorStateSubsystem.h"

EDSPlaceType UDSIndoorStateSubsystem::GetAreaState() const
{
	return CurrentAreaState;
}

void UDSIndoorStateSubsystem::SetAreaState(EDSPlaceType NewState)
{
	if (CurrentAreaState == NewState)
	{
		return;
	}

	CurrentAreaState = NewState;
	OnAreaStateChanged.Broadcast(CurrentAreaState);
}
