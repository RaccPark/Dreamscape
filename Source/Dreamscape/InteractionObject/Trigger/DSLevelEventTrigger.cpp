// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/Trigger/DSLevelEventTrigger.h"
#include "GameMode/DSGameState.h"
#include "Interface/DSActivatableInterface.h"

// Sets default values
ADSLevelEventTrigger::ADSLevelEventTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADSLevelEventTrigger::BeginPlay()
{
	Super::BeginPlay();

	ADSGameState* GameState = GetWorld()->GetGameState<ADSGameState>();
	if (!GameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ADSLevelEventTrigger] GameState was nullptr!"))
		return;
	}

	GameState->OnAllCrystalsDestroyedDelegate.AddDynamic(this, &ADSLevelEventTrigger::HandleAllCrystalsDestroyed);
}

void ADSLevelEventTrigger::HandleAllCrystalsDestroyed()
{
	UE_LOG(LogTemp, Warning, TEXT("LevelEventTrigger Activated"));

	ActivateTargets();
}

void ADSLevelEventTrigger::ActivateTargets()
{
	for (AActor* Target : Targets)
	{
		if (!Target)
		{
			continue;
		}

		IDSActivatableInterface* ActivatableActor = Cast<IDSActivatableInterface>(Target);
		if (!ActivatableActor)
		{
			continue;
		}
		ActivatableActor->Activate();
	}
}

