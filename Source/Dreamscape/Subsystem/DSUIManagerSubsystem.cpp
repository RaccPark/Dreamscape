// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DSUIManagerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/DSPlayerController.h"

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"

UDSUIManagerSubsystem::UDSUIManagerSubsystem()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_InGameMenu.WBP_InGameMenu_C'"));
	if (WidgetClass.Succeeded())
	{
		InGamePauseMenuClass = WidgetClass.Class;
	}

	bIsPaused = false;
}

void UDSUIManagerSubsystem::PauseGame()
{

	ADSPlayerController* PlayerController = Cast<ADSPlayerController>(GetLocalPlayer()->GetPlayerController(GetWorld()));
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not found in TogglePauseMenu"));
		return;
	}

	IMC_Default = PlayerController->GetDefaultInputMappingContext();
	IMC_Pause = PlayerController->GetPauseInputMappingContext();

	// 위젯 생성 (한 번만)
	if (!InGamePauseMenuWidget && InGamePauseMenuClass)
	{
		InGamePauseMenuWidget = CreateWidget<UUserWidget>(PlayerController, InGamePauseMenuClass);
	}

	if (InGamePauseMenuWidget && !InGamePauseMenuWidget->IsInViewport())
	{
		InGamePauseMenuWidget->AddToViewport(100);
	}

	// IMC_Default 제거
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		if (IMC_Default)
		{
			Subsystem->RemoveMappingContext(IMC_Default);
		}

		if (IMC_Pause)
		{
			Subsystem->AddMappingContext(IMC_Pause, 10);
		}
	}

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	// 입력 UI로 변경
	PlayerController->SetInputMode(FInputModeGameAndUI());
	PlayerController->bShowMouseCursor = true;


	bIsPaused = true;
}

void UDSUIManagerSubsystem::ResumeGame()
{
	APlayerController* PlayerController = GetLocalPlayer()->GetPlayerController(GetWorld());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not found in TogglePauseMenu"));
		return;
	}

	if (InGamePauseMenuWidget && InGamePauseMenuWidget->IsInViewport())
	{
		InGamePauseMenuWidget->RemoveFromParent();
	}

	// IMC_Default 추가
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		if (IMC_Pause)
		{
			Subsystem->RemoveMappingContext(IMC_Pause);
		}

		if (IMC_Default)
		{
			Subsystem->AddMappingContext(IMC_Default, 0);
		}
	}

	UGameplayStatics::SetGamePaused(GetWorld(), false);

	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;

	bIsPaused = false;
}

void UDSUIManagerSubsystem::TogglePauseMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("TogglePauseMenu called in UDSUIManagerSubsystem"));

	if (bIsPaused)
	{
		ResumeGame();
	}
	else
	{
		PauseGame();
	}
}
