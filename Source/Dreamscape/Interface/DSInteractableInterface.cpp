// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/DSInteractableInterface.h"

// Add default functionality here for any IDSInteractableInterface functions that are not pure virtual.

FString IDSInteractableInterface::GetInteractPrompt() const
{
	return TEXT("Interact");
}
