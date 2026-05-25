// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/Trigger/DSBossTrigger.h"
#include "Components/BoxComponent.h"
#include "DSBossEncounterManager.h"

ADSBossTrigger::ADSBossTrigger()
{

}

void ADSBossTrigger::BeginPlay()
{
	Super::BeginPlay();

	OnTriggerActivatedDelegate.AddDynamic(this, &ADSBossTrigger::HandleTriggered);
}

void ADSBossTrigger::HandleTriggered(AActor* TriggeringActor)
{
	if (!EncounterManager)
	{
		return;
	}

	EncounterManager->StartEncounter();
}
