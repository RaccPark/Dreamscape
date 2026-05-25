// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DSPlayerController.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Character/DSCharacterPlayer.h"

#include "Subsystem/DSUIManagerSubsystem.h"

#include "UI/DSPlayerHUDWidget.h"
#include "UI/DSResourceBarWidget.h"

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

	// UI
	if (PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<UDSPlayerHUDWidget>(this, PlayerHUDWidgetClass);
		if (PlayerHUDWidget)
		{
			// Controller에서 조종중인 캐릭터를 가져올 때에는 GetPawn()으로 가져온다.
			ADSCharacterPlayer* CharacterPlayer = Cast<ADSCharacterPlayer>(GetPawn());
			if (CharacterPlayer && PlayerHUDWidget)
			{
				PlayerHUDWidget->WBPResourceBar->BindToPlayer(CharacterPlayer);

				int32 RoundedHealth = FMath::RoundToInt32(CharacterPlayer->GetCurrentHealth());
				PlayerHUDWidget->WBPResourceBar->UpdateHealth(RoundedHealth);
			}
			else if (!CharacterPlayer)
			{
				UE_LOG(LogTemp, Warning, TEXT("CharacterPlayer was nullptr!"));
			}
			else if (!PlayerHUDWidget)
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerHUDWidget was nullptr!"));
			}

			PlayerHUDWidget->AddToViewport();
		}
	}
}

void ADSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ADSPlayerController::ApplyGameplayMode()
{
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	SetShowMouseCursor(false);

	ADSCharacterBase* CharacterPlayer = Cast<ADSCharacterBase>(GetPawn());
	if (!CharacterPlayer)
	{
		return;
	}

	auto* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem)
	{
		return;
	}

	Subsystem->RemoveMappingContext(CinematicInputMappingContext);
	Subsystem->RemoveMappingContext(PasueInputMappingContext);

	if (DefaultInputMappingContext)
	{
		UE_LOG(LogTemp, Warning, TEXT("Adding DefaultInputMappingContext to Subsystem"));
		Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
	}

	CurrentInputMode = EPlayerInputMode::Gameplay;
}

void ADSPlayerController::ApplyCinematicMode()
{
	ADSCharacterPlayer* CharacterPlayer = Cast<ADSCharacterPlayer>(GetPawn());
	if (!CharacterPlayer)
	{
		return;
	}

	auto* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem)
	{
		return;
	}

	Subsystem->RemoveMappingContext(CharacterPlayer->GetDefaultMappingContext());
	Subsystem->RemoveMappingContext(PasueInputMappingContext);

	SetShowMouseCursor(false);

	FInputModeGameOnly InputMode;
	Super::SetInputMode(InputMode);

	CurrentInputMode = EPlayerInputMode::Cinematic;
}

void ADSPlayerController::ApplyUIMode()
{
	OnPausePressed();

	CurrentInputMode = EPlayerInputMode::UI;
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

void ADSPlayerController::ChangeInputMode(EPlayerInputMode NewInputMode)
{
	if (CurrentInputMode == NewInputMode)
	{
		return;
	}

	switch (NewInputMode)
	{
	case EPlayerInputMode::Gameplay:
		ApplyGameplayMode();
		break;
	case EPlayerInputMode::UI:
		ApplyUIMode();
		break;
	case EPlayerInputMode::Cinematic:
		ApplyCinematicMode();
		break;
	}
}

EPlayerInputMode ADSPlayerController::GetCurrentInputMode() const
{
	return CurrentInputMode;
}
