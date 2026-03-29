// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/DSEnemyCharacterBase.h"

// Sets default values
ADSEnemyCharacterBase::ADSEnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



}

void ADSEnemyCharacterBase::TakeDamage(float DamageAmount)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0.0f)
	{
		OnDeath();
	}
}

// Called when the game starts or when spawned
void ADSEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADSEnemyCharacterBase::OnDeath()
{
	// Play death animation
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
}

// Called every frame
void ADSEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADSEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

