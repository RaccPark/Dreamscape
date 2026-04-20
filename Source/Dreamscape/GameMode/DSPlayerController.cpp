// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DSPlayerController.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Character/DSCharacterPlayer.h"

#include "Subsystem/DSUIManagerSubsystem.h"

void ADSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (LocalPlayer)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (Subsystem)
		{
			// Subsystem->AddMappingContext(PasueInputMappingContext, 1);
		}

		// Get GamePlayInputMappingContext from Character and add it to Input Subsystem
		APawn* ControlledPawn = GetPawn();
		if (ControlledPawn)
		{
			ADSCharacterPlayer* CharacterPlayer = Cast<ADSCharacterPlayer>(ControlledPawn);
			if (CharacterPlayer && CharacterPlayer->GetDefaultMappingContext())
			{
				DefaultInputMappingContext = CharacterPlayer->GetDefaultMappingContext();
			}
		}
	}
}

void ADSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		if (ResumeAction)
		{
			ResumeAction->bTriggerWhenPaused = true;
			EnhancedInputComponent->BindAction(ResumeAction, ETriggerEvent::Started, this, &ADSPlayerController::OnPausePressed);
		}
	}
}

void ADSPlayerController::OnPausePressed()
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (LocalPlayer)
	{
		UDSUIManagerSubsystem* UIManager = LocalPlayer->GetSubsystem<UDSUIManagerSubsystem>();
		if (UIManager)
		{
			UIManager->TogglePauseMenu();
		}
	}
}

UInputMappingContext* ADSPlayerController::GetPauseInputMappingContext() const
{
	return PasueInputMappingContext;
}

UInputMappingContext* ADSPlayerController::GetDefaultInputMappingContext() const
{
	return DefaultInputMappingContext;
}
