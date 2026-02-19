// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Enum/DSPlaceType.h"
#include "GameplayTagContainer.h"
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

	UPROPERTY(EditAnywhere)
	float TransitionDuration;

private:
	TMap<FGameplayTag, TArray<UMaterialInstanceDynamic*>> RoomMIDs;

	FTimerHandle TransitionTimerHandle;

	TMap<FGameplayTag, float> CurrentRoomAlpha;
	TMap<FGameplayTag, float> TargetRoomAlpha;
	FGameplayTag ActiveRoomTag;

	UFUNCTION()
	void StartTransition(FGameplayTag ActiveRoom);

	void UpdateTransition();

	void ApplyFadeToRoom(const FGameplayTag& RoomTag, float Alpha);

	void CacheMIDs();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
