// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DSAttackTraceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDSAttackTraceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DREAMSCAPE_API IDSAttackTraceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void StartAttackTrace() = 0;
	virtual void PerformAttackTrace() = 0;
	virtual void EndAttackTrace() = 0;
};
