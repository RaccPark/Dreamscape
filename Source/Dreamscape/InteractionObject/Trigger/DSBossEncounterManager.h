// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/DSBossEncounterState.h"
#include "DSBossEncounterManager.generated.h"

UCLASS()
class DREAMSCAPE_API ADSBossEncounterManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADSBossEncounterManager();

	UFUNCTION()
	void StartEncounter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss Encounter")
	EDSBossEncounterState CurrentEncounterState;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Encounter")
	TArray<AActor*> InteractionTargets;

	void CloseDoors();
	void OnDoorsClosed();

	// Boss
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Encounter")
	TSubclassOf<AActor> BossClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Encounter")
	TObjectPtr<class USceneComponent> BossSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Encounter")
	TObjectPtr<class UArrowComponent> BossFacingDirection;

	// Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Encounter|Camera")
	TObjectPtr<class ADSCameraDirector> BossIntroCameraDirector;

	TObjectPtr<class ADSBossCharacterBase> SpawnedBoss;

	void SpawnBoss();
	void SetupBoss();
	void StartCameraTransition();

	UFUNCTION()
	void OnBossLandingFinished();

	void StartCombat();
	UFUNCTION()
	void OnBossDead();

	void FinishEncounter();
	void OpenDoors();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Encounter|TimerSetting")
	float DelayAfterDoorClose;

	// Timer Handles
	FTimerHandle BossSetupTimerHandle;
	FTimerHandle DelayAfterDoorCloseTimerHandle;

};
