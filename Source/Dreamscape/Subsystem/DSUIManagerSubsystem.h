// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "DSUIManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSUIManagerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
public:
	UDSUIManagerSubsystem();
	
protected:
	void PauseGame();
	void ResumeGame();

public:
	void TogglePauseMenu();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> IMC_Default;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> IMC_Pause;

	UPROPERTY()
	TSubclassOf<class UUserWidget> InGamePauseMenuClass;

	UPROPERTY()
	TObjectPtr<class UUserWidget> InGamePauseMenuWidget;

	bool bIsPaused;
};
