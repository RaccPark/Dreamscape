// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Enum/DSPlaceType.h"
#include "DSIndoorVisibilityManager.generated.h"

UCLASS()
class DREAMSCAPE_API ADSIndoorVisibilityManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADSIndoorVisibilityManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnLevelLoaded();

	UPROPERTY(VisibleAnywhere, Category = "Sub Level")
	FName OutdoorMeshTag;

	UPROPERTY(VisibleAnywhere, Category = "Sub Level")
	FName IndoorMeshTag;

	UPROPERTY(EditAnywhere)
	float TransitionDuration;

private:
	UPROPERTY()
	TArray<class UMaterialInstanceDynamic*> IndoorMIDs;
	UPROPERTY()
	TArray<class UMaterialInstanceDynamic*> OutdoorMIDs;

	FTimerHandle TransitionTimerHandle;

	float CurrentAlpha;
	float TargetAlpha;

	void ApplyVisibility(EDSPlaceType State);

	UFUNCTION()
	void StartTransition(EDSPlaceType State);
	void UpdateTransition();
	void ApplyFade(float Alpha);

	void CacheMIDs();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
