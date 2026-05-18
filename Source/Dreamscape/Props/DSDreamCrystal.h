// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/DSDamageableInterface.h"
#include "DSDreamCrystal.generated.h"

UENUM()
enum class ECrystalState : uint8
{
	Unlocked,
	Locked,
	Destroyed
};

UCLASS()
class DREAMSCAPE_API ADSDreamCrystal : public AActor, public IDSDamageableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADSDreamCrystal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "State")
	ECrystalState CrystalState;

	bool bDestroyed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ApplyDamage(float DamageAmount) override;
	virtual void ApplyDamageWithKnockback(float DamageAmount, const FVector& KnockbackDirection, float KnockbackStrength) override;

private:
	void DestroyCrystal();
};
