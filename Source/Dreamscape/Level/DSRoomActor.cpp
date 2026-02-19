// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/DSRoomActor.h"

// Sets default values
ADSRoomActor::ADSRoomActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADSRoomActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADSRoomActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

