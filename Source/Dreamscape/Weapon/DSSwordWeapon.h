// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DSSwordWeapon.generated.h"

UCLASS()
class DREAMSCAPE_API ADSSwordWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADSSwordWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> SwordWeaponMesh;

	bool bTraceAtive;

	TArray<AActor*> HitActors;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartAttackTrace();
	void EndAttackTrace();
	void PerformTrace();

};
