// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DSTriggerBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggerActivated, AActor*, TriggeringActor);

UCLASS()
class DREAMSCAPE_API ADSTriggerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADSTriggerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> TriggerBox;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FOnTriggerActivated OnTriggerActivatedDelegate;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	uint8 bTriggerOnce : 1;

	bool bAlreadyTriggered;

	// 특정 타입만 허용하기 위한 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	TSubclassOf<AActor> AllowedActorClass;

};
