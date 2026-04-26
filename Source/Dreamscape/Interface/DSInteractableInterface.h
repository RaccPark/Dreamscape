// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DSInteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDSInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DREAMSCAPE_API IDSInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Interact(class ADSCharacterPlayer* Player) = 0;
	virtual FString GetInteractPrompt() const;

	virtual void OnFocus() {}
	virtual void OnFocusLost() {}
};
