// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/DSCameraDirector.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

// Sets default values
ADSCameraDirector::ADSCameraDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BlendTime = 1.0f;
	CutsceneDuration = 3.0f;
}

// Called when the game starts or when spawned
void ADSCameraDirector::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADSCameraDirector::ReturnToPlayer()
{
	if (!PlayerController || !PlayerPawn)
	{
		return;
	}

	PlayerController->SetViewTargetWithBlend(PlayerPawn, BlendTime);
	PlayerController->SetIgnoreMoveInput(false);
	PlayerController->SetIgnoreLookInput(false);
}

// Called every frame
void ADSCameraDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADSCameraDirector::Activate()
{
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController || !TargetCamera)
	{
		return;
	}

	PlayerController->SetViewTargetWithBlend(TargetCamera, BlendTime);
	PlayerController->SetIgnoreMoveInput(true);
	PlayerController->SetIgnoreLookInput(true);

	PlayerPawn = PlayerController->GetPawn();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,
		this,
		&ADSCameraDirector::ReturnToPlayer,
		CutsceneDuration,
		false);
}

