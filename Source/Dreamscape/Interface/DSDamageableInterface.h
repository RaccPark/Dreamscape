// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Types/DSDamageTypes.h"
#include "DSDamageableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDSDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DREAMSCAPE_API IDSDamageableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 피해량·종류·공격자·넉백을 한 번에 받는다. 넉백이 없으면 KnockbackStrength가 0이다 (HasKnockback()으로 확인)
	virtual void ApplyDamage(const FDSDamageInfo& DamageInfo) = 0;
};
