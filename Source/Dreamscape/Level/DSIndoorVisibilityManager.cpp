// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/DSIndoorVisibilityManager.h"
#include "EngineUtils.h"
#include "Components/MeshComponent.h"
#include "../Subsystem/DSIndoorStateSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayTagAssetInterface.h"

// Sets default values
ADSIndoorVisibilityManager::ADSIndoorVisibilityManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TransitionDuration = 0.50f;
}

// Called when the game starts or when spawned
void ADSIndoorVisibilityManager::BeginPlay()
{
	Super::BeginPlay();

	CacheMIDs();

	for (const auto& Pair : RoomMIDs)
	{
		CurrentRoomAlpha.Add(Pair.Key, 0.0f);
		TargetRoomAlpha.Add(Pair.Key, 0.0f);
	}

	if (UDSIndoorStateSubsystem* IndoorSubsystem = GetGameInstance()->GetSubsystem<UDSIndoorStateSubsystem>())
	{
		IndoorSubsystem->OnRoomChanged.AddUObject(this, &ADSIndoorVisibilityManager::StartTransition);

		// Initial Room은 항상 Area.Room.Start로 시작함
		IndoorSubsystem->SetCurrentRoom(FGameplayTag::RequestGameplayTag(FName("Area.Room.Start")));
	}
}

void ADSIndoorVisibilityManager::StartTransition(FGameplayTag NewRoom)
{
	GetWorldTimerManager().ClearTimer(TransitionTimerHandle);

	ActiveRoomTag = NewRoom;
	UE_LOG(LogTemp, Warning, TEXT("Starting transition to room: %s"), *ActiveRoomTag.ToString());

	for (auto& Pair : TargetRoomAlpha)
	{
		const bool bIsActive = (Pair.Key == ActiveRoomTag);
		Pair.Value = bIsActive ? 1.0f : 0.0f;
		UE_LOG(LogTemp, Warning, TEXT("Setting target alpha for room %s to %.1f"), *Pair.Key.ToString(), Pair.Value);
	}

	GetWorldTimerManager().SetTimer(TransitionTimerHandle, this, &ADSIndoorVisibilityManager::UpdateTransition, 0.016f, true);
}

void ADSIndoorVisibilityManager::UpdateTransition()
{
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	const float InterpSpeed = 1.0f / TransitionDuration;

	bool bAllFinished = true;

	for (auto& Pair : CurrentRoomAlpha)
	{
		const FGameplayTag& RoomTag = Pair.Key;

		float& Current = Pair.Value;
		float Target = TargetRoomAlpha[RoomTag];

		Current = FMath::FInterpTo(Current, Target, DeltaTime, InterpSpeed * 5.0f);

		ApplyFadeToRoom(RoomTag, Current);

		if (!FMath::IsNearlyEqual(Current, Target, 0.01f))
		{
			bAllFinished = false;
		}
	}
}

void ADSIndoorVisibilityManager::ApplyFadeToRoom(const FGameplayTag& RoomTag, float Alpha)
{
	if (!RoomMIDs.Contains(RoomTag))
	{
		return;
	}

	for (UMaterialInstanceDynamic* MID : RoomMIDs[RoomTag])
	{
		if (MID)
		{
			MID->SetScalarParameterValue("Fade", Alpha);
		}
	}
}

// Area.Room.Corridor1, Area.Room.Corridor2, Area.Room.Lobby 등과 같이 Room 태그를 가진 Actor들의 MID를 캐싱하는 함수
// Area.Room.Corridor -> [MID1, MID2, MID3]
// Area.Room.Hall -> [MID4, MID5]	
// Area.Room.Yard -> [MID7, MID8, MID9]
// 같이 묶임
void ADSIndoorVisibilityManager::CacheMIDs()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);

	const FGameplayTag RoomRootTag =
		FGameplayTag::RequestGameplayTag(FName("Area.Room"));

	for (AActor* Actor : Actors)
	{
		if (!Actor)
		{
			continue;
		}

		// GameplayTag 인터페이스 검사
		if (!Actor->GetClass()->ImplementsInterface(UGameplayTagAssetInterface::StaticClass()))
		{
			continue;
		}

		// Actor가 가진 GameplayTag 수집
		FGameplayTagContainer TagContainer;
		IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Actor);

		if (!TagInterface)
		{
			continue;
		}

		TagInterface->GetOwnedGameplayTags(TagContainer);

		// Area.Room.* 태그 찾기
		FGameplayTag FoundRoomTag;

		for (const FGameplayTag& Tag : TagContainer)
		{
			if (Tag.MatchesTag(RoomRootTag))
			{
				FoundRoomTag = Tag;
				break;
			}
		}

		if (!FoundRoomTag.IsValid())
		{
			continue;
		}

		// MID 캐싱
		TArray<UStaticMeshComponent*> MeshComponents;
		Actor->GetComponents<UStaticMeshComponent>(MeshComponents);

		for (UStaticMeshComponent* Mesh : MeshComponents)
		{
			if (!Mesh)
			{
				continue;
			}

			const int32 MaterialCount = Mesh->GetNumMaterials();

			for (int32 Index = 0; Index < MaterialCount; Index++)
			{
				UMaterialInstanceDynamic* MID =
					Mesh->CreateAndSetMaterialInstanceDynamic(Index);

				if (MID)
				{
					RoomMIDs.FindOrAdd(FoundRoomTag).Add(MID);
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Cached Room Count: %d"), RoomMIDs.Num());
}

// Called every frame
void ADSIndoorVisibilityManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

