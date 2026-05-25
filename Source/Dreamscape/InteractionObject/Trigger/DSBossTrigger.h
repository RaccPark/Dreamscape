// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionObject/Trigger/DSTriggerBase.h"
#include "DSBossTrigger.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API ADSBossTrigger : public ADSTriggerBase
{
	GENERATED_BODY()
	
public:
	ADSBossTrigger();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss|Trigger")
	TObjectPtr<class ADSBossEncounterManager> EncounterManager;
	

	UFUNCTION()
	void HandleTriggered(AActor* TriggeringActor);
};
