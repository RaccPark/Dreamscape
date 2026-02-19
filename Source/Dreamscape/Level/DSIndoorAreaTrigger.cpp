// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/DSIndoorAreaTrigger.h"
#include "Components/BoxComponent.h"
#include "Level/DSIndoorVisibilityManager.h"
#include "../Subsystem/DSIndoorStateSubsystem.h"
#include "../Enum/DSPlaceType.h"

// Sets default values
ADSIndoorAreaTrigger::ADSIndoorAreaTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);

	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADSIndoorAreaTrigger::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ADSIndoorAreaTrigger::BeginPlay()
{
	Super::BeginPlay();

}

void ADSIndoorAreaTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor || !OtherActor->ActorHasTag("Player"))
	{
		return;
	}

	if (!RoomTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("RoomTag not set on %s"), *GetName());
		return;
	}

	if (UDSIndoorStateSubsystem* IndoorStateSubsystem = GetGameInstance()->GetSubsystem<UDSIndoorStateSubsystem>())
	{
		IndoorStateSubsystem->SetCurrentRoom(RoomTag);
	}

	UE_LOG(LogTemp, Warning, TEXT("Entered Room: %s"), *RoomTag.ToString());
}

// Called every frame
void ADSIndoorAreaTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

