// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DSCharacterComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSCharacterComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UDSCharacterComboActionData();

	UPROPERTY(EditAnywhere, Category = "Name")
	FString MontageSectionNameProfix;

	UPROPERTY(EditAnywhere, Category = "Name")
	uint32 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = "Name")
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = "Name")
	TArray<float> EffectiveFrameCounts;
};
