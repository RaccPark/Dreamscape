// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/DSEnemyAnimInstance.h"
#include "DSBoss01AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSBoss01AnimInstance : public UDSEnemyAnimInstance
{
	GENERATED_BODY()
	
public:
	UDSBoss01AnimInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bWasLanded : 1;


};
