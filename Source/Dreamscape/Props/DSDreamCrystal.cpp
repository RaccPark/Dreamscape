// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/DSDreamCrystal.h"
#include "GameMode/DSGameState.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
ADSDreamCrystal::ADSDreamCrystal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DreamCrystalMesh"));
	SetRootComponent(StaticMeshComponent);


	CrystalState = ECrystalState::Unlocked;
	bDestroyed = false;
}

// Called when the game starts or when spawned
void ADSDreamCrystal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADSDreamCrystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADSDreamCrystal::ApplyDamage(float DamageAmount)
{
	if (bDestroyed)
	{
		return;
	}

	if (CrystalState == ECrystalState::Locked)
	{
		return;
	}

	DestroyCrystal();
}

void ADSDreamCrystal::ApplyDamageWithKnockback(float DamageAmount, const FVector& KnockbackDirection, float KnockbackStrength)
{
	ApplyDamage(DamageAmount);
}

void ADSDreamCrystal::DestroyCrystal()
{
	UE_LOG(LogTemp, Warning, TEXT("Crystal Destroyed!"));
	bDestroyed = true;
	CrystalState = ECrystalState::Destroyed;

	ADSGameState* GameState = GetWorld()->GetGameState<ADSGameState>();
	if (GameState)
	{
		GameState->OnCrystalDestroyed();
	}

	// FX
	// ...

	// 제거
	// 나중에 조각나게 할 예정
	Destroy();
}

