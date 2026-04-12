// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DSSwordWeaponBase.generated.h"

UCLASS()
class DREAMSCAPE_API ADSSwordWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADSSwordWeaponBase();

	virtual void InitializeWeapon(const class UDSWeaponItemData* WeaponData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> SwordWeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float DamageAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AttackRate;

	TArray<AActor*> HitActors;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartAttackTrace();
	void EndAttackTrace();
	void PerformTrace();

};
