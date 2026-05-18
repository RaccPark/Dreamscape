// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DSLevelEventTrigger.generated.h"

UCLASS()
class DREAMSCAPE_API ADSLevelEventTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADSLevelEventTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Event")
	TArray<AActor*> Targets;

private:
	UFUNCTION()
	void HandleAllCrystalsDestroyed();

	void ActivateTargets();
};
