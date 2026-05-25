// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/Trigger/DSBossEncounterManager.h"
#include "Interface/DSActivatableInterface.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/Enemy/DSBoss01.h"
#include "InteractionObject/DSCameraDirector.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ADSBossEncounterManager::ADSBossEncounterManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BossSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("BossSpawnPoint"));
	BossSpawnPoint->SetupAttachment(RootComponent);

	BossFacingDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("BossFacingDirection"));
	BossFacingDirection->SetupAttachment(BossSpawnPoint);

	DelayAfterDoorClose = 0.25f;

}

void ADSBossEncounterManager::StartEncounter()
{
	if (CurrentEncounterState != EDSBossEncounterState::BES_Idle)
	{
		return;
	}

	CurrentEncounterState = EDSBossEncounterState::BES_DoorClosing;

	CloseDoors();
}

// Called when the game starts or when spawned
void ADSBossEncounterManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADSBossEncounterManager::CloseDoors()
{
	for (AActor* Target : InteractionTargets)
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

	// After Door Timer
	GetWorldTimerManager().SetTimer(DelayAfterDoorCloseTimerHandle, this, &ADSBossEncounterManager::OnDoorsClosed, DelayAfterDoorClose, false);
}

void ADSBossEncounterManager::OnDoorsClosed()
{
	SpawnBoss();

}

void ADSBossEncounterManager::SpawnBoss()
{
	CurrentEncounterState = EDSBossEncounterState::BES_BossSpawning;

	if (!BossClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("BossClass was not set in BossEncounterManager!"));
		return;
	}

	SpawnedBoss = GetWorld()->SpawnActor<ADSBossCharacterBase>(BossClass, BossSpawnPoint->GetComponentTransform());

	if (!SpawnedBoss)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn boss in BossEncounterManager!"));
		return;
	}

	CurrentEncounterState = EDSBossEncounterState::BES_WaitingForLanding;

	// ...
	SpawnedBoss->OnBossLandingFinishedDelegate.AddDynamic(this, &ADSBossEncounterManager::OnBossLandingFinished);
	SpawnedBoss->OnDeathDelegate.AddDynamic(this, &ADSBossEncounterManager::OnBossDead);
	
	// 한 프레임 뒤에 SetupBoss를 호출하여 Boss의 초기 설정을 합니다. (낙하 중인 보스의 물리 설정 등)
	GetWorldTimerManager().SetTimer(BossSetupTimerHandle, this, &ADSBossEncounterManager::SetupBoss, 0.1f, false);

	// 보스 컷신
	StartCameraTransition();
}

void ADSBossEncounterManager::SetupBoss()
{
	UCharacterMovementComponent* BossMovement = SpawnedBoss->GetCharacterMovement();
	if (BossMovement)
	{
		BossMovement->SetMovementMode(EMovementMode::MOVE_Falling);
		BossMovement->GravityScale = 1.0f;

		BossMovement->Velocity = FVector(0.0f, 0.0f, -100.0f);
	}
}

void ADSBossEncounterManager::StartCameraTransition()
{
	if (BossIntroCameraDirector)
	{
		BossIntroCameraDirector->Activate();
	}
}

void ADSBossEncounterManager::OnBossLandingFinished()
{
	StartCombat();
}

void ADSBossEncounterManager::StartCombat()
{
	CurrentEncounterState = EDSBossEncounterState::BES_InCombat;

	if (SpawnedBoss)
	{
		// Activate Boss AI, combat music, etc
		// ...
	}
}

void ADSBossEncounterManager::OnBossDead()
{
	UE_LOG(LogTemp, Log, TEXT("Boss has died!"));

	if (CurrentEncounterState != EDSBossEncounterState::BES_InCombat)
	{
		return;
	}

	CurrentEncounterState = EDSBossEncounterState::BES_BossDead;

	FinishEncounter();
}

void ADSBossEncounterManager::FinishEncounter()
{
	CurrentEncounterState = EDSBossEncounterState::BES_Finished;

	OpenDoors();

	// 보스전 후
}

void ADSBossEncounterManager::OpenDoors()
{
	for (AActor* Target : InteractionTargets)
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

// Called every frame
void ADSBossEncounterManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

