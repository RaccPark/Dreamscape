// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/DSActivatableInterface.h"
#include "Components/TimelineComponent.h"
#include "DSDoor.generated.h"

UCLASS()
class DREAMSCAPE_API ADSDoor : public AActor, public IDSActivatableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADSDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> DoorMesh;

	bool bIsOpen;
	bool bIsMoving;

	UPROPERTY(EditAnywhere)
	FRotator ClosedRotation;

	UPROPERTY(EditAnywhere)
	FRotator OpenRotation;

	FTimeline DoorTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	TObjectPtr<class UCurveFloat> DoorCurve;

	UFUNCTION()
	void UpdateDoorRotation(float Alpha);

	UFUNCTION()
	void OnTimelineFinished();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Activate() override;

};
