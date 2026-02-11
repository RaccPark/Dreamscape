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
	PrimaryActorTick.bCanEverTick = false;

	OutdoorMeshTag = "Outdoor";
	IndoorMeshTag = "Indoor";

	TransitionDuration = 0.50f;
	CurrentAlpha = 0.0f;
	TargetAlpha = 0.0f;
}

// Called when the game starts or when spawned
void ADSIndoorVisibilityManager::BeginPlay()
{
	Super::BeginPlay();

	CacheMIDs();

	UDSIndoorStateSubsystem* IndoorStateSubsystem = GetGameInstance()->GetSubsystem<UDSIndoorStateSubsystem>();
	if (IndoorStateSubsystem)
	{
		IndoorStateSubsystem->OnAreaStateChanged.AddUObject(this, &ADSIndoorVisibilityManager::StartTransition);

		// Apply initial visibility based on current state
		StartTransition(IndoorStateSubsystem->GetAreaState());
	}

	UE_LOG(LogTemp, Warning, TEXT("Indoor MID Count: %d"), IndoorMIDs.Num());
	UE_LOG(LogTemp, Warning, TEXT("Outdoor MID Count: %d"), OutdoorMIDs.Num());

}

void ADSIndoorVisibilityManager::StartTransition(EDSPlaceType State)
{
	TargetAlpha = (State == EDSPlaceType::EPT_Indoor) ? 1.0f : 0.0f;

	GetWorldTimerManager().SetTimer(TransitionTimerHandle, this, &ADSIndoorVisibilityManager::UpdateTransition, 0.016f, true);
}

void ADSIndoorVisibilityManager::UpdateTransition()
{
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	const float InterpSpeed = 1.0f / TransitionDuration;	
	
	CurrentAlpha = FMath::FInterpTo(CurrentAlpha, TargetAlpha, DeltaTime, InterpSpeed * 5.0f);

	ApplyFade(CurrentAlpha);

	if(FMath::IsNearlyEqual(CurrentAlpha, TargetAlpha, 0.01f))
	{
		CurrentAlpha = TargetAlpha;
		ApplyFade(CurrentAlpha);
		GetWorldTimerManager().ClearTimer(TransitionTimerHandle);
	}

	UE_LOG(LogTemp, Warning, TEXT("Alpha: %f"), CurrentAlpha);

}

void ADSIndoorVisibilityManager::ApplyFade(float Alpha)
{
	for (UMaterialInstanceDynamic* MID : IndoorMIDs)
	{
		if (MID)
		{
			MID->SetScalarParameterValue("Fade", Alpha);
		}
	}

	for (UMaterialInstanceDynamic* MID : OutdoorMIDs)
	{
		if (MID)
		{
			MID->SetScalarParameterValue("Fade", 1.0f - Alpha);
		}
	}
}

void ADSIndoorVisibilityManager::CacheMIDs()
{
	TArray<AActor*> IndoorActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), IndoorMeshTag, IndoorActors);

	for (AActor* Actor : IndoorActors)
	{
		if (!Actor)
		{
			continue;
		}

		TArray<UStaticMeshComponent*> MeshComponents;
		Actor->GetComponents<UStaticMeshComponent>(MeshComponents);

		for (auto* Mesh : MeshComponents)
		{
			if (!Mesh)
			{
				continue;
			}
			
			const int32 MaterialCount = Mesh->GetNumMaterials();

			for (int32 i = 0; i < MaterialCount; i++)
			{
				UMaterialInstanceDynamic* MID = Mesh->CreateAndSetMaterialInstanceDynamic(i);

				if (MID)
				{
					IndoorMIDs.Add(MID);
				}
			}

		}
	}

	TArray<AActor*> OutdoorActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), OutdoorMeshTag, OutdoorActors);

	for (AActor* Actor : OutdoorActors)
	{
		if (!Actor)
		{
			continue;
		}
		TArray<UStaticMeshComponent*> MeshComponents;
		Actor->GetComponents<UStaticMeshComponent>(MeshComponents);
		for (auto* Mesh : MeshComponents)
		{
			if (!Mesh)
			{
				continue;
			}
			const int32 MaterialCount = Mesh->GetNumMaterials();

			for (int32 i = 0; i < MaterialCount; i++)
			{
				UMaterialInstanceDynamic* MID = Mesh->CreateAndSetMaterialInstanceDynamic(i);

				if (MID)
				{
					OutdoorMIDs.Add(MID);
				}
			}
		}
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

		TArray<UStaticMeshComponent*> MeshComponents;
		Actor->GetComponents<UStaticMeshComponent>(MeshComponents);

		for (auto* Mesh : MeshComponents)
		{
			if (!Mesh)
			{
				continue;
			}

			UMaterialInstanceDynamic* DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamic(0);

			if (DynamicMaterial)
			{
				//DynamicMaterial->SetScalarParameterValue("Fade", Alpha);
			}
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

		//Actor->SetActorHiddenInGame(bIsIndoor);

	}
}

// Called every frame
void ADSIndoorVisibilityManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

