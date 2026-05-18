// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/DSActivatableInterface.h"
#include "DSCameraDirector.generated.h"

UCLASS()
class DREAMSCAPE_API ADSCameraDirector : public AActor, public IDSActivatableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADSCameraDirector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<AActor> TargetCamera;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float BlendTime;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CutsceneDuration;

	UFUNCTION()
	void ReturnToPlayer();

private:
	APlayerController* PlayerController;
	APawn* PlayerPawn;

	FTimerHandle TimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Activate() override;

};
