// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/DSIndoorVisibilityManager.h"
#include "EngineUtils.h"
#include "Components/MeshComponent.h"
#include "../Subsystem/DSIndoorStateSubsystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ADSIndoorVisibilityManager::ADSIndoorVisibilityManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OutdoorMeshTag = "Outdoor";
	IndoorMeshTag = "Indoor";
}

// Called when the game starts or when spawned
void ADSIndoorVisibilityManager::BeginPlay()
{
	Super::BeginPlay();

	UDSIndoorStateSubsystem* IndoorStateSubsystem = GetGameInstance()->GetSubsystem<UDSIndoorStateSubsystem>();
	if (IndoorStateSubsystem)
	{
		IndoorStateSubsystem->OnAreaStateChanged.AddUObject(this, &ADSIndoorVisibilityManager::ApplyVisibility);

		// Apply initial visibility based on current state
		ApplyVisibility(IndoorStateSubsystem->GetAreaState());
	}
}

void ADSIndoorVisibilityManager::ApplyVisibility(EDSPlaceType State)
{
	const bool bIsIndoor = (State == EDSPlaceType::EPT_Indoor);

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), IndoorMeshTag, Actors);

	// Set Indoor Meshes Visibility
	for (AActor* Actor : Actors)
	{
		if (!Actor)
		{
			continue;
		}

		Actor->SetActorHiddenInGame(!bIsIndoor);
	}

	Actors.Reset();
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), OutdoorMeshTag, Actors);

	// Set Outdoor Meshes Visibility
	for (AActor* Actor : Actors)
	{
		if (!Actor)
		{
			continue;
		}

		Actor->SetActorHiddenInGame(bIsIndoor);
	}
}

// Called every frame
void ADSIndoorVisibilityManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

