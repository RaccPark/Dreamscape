// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DSPlayerController.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerInputMode : uint8
{
	Gameplay    UMETA(DisplayName = "Gameplay"),	// 일반 조작
	Cinematic   UMETA(DisplayName = "Cinematic"),   // 카메라 연출 - 입력 차단
	UI          UMETA(DisplayName = "UI"),          // UI만 입력
};

UCLASS()
class DREAMSCAPE_API ADSPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> PasueInputMappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> CinematicInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ResumeAction;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UDSPlayerHUDWidget> PlayerHUDWidgetClass;

	UPROPERTY()
	TObjectPtr<class UDSPlayerHUDWidget> PlayerHUDWidget;

	EPlayerInputMode CurrentInputMode;

	void ApplyGameplayMode();
	void ApplyCinematicMode();
	void ApplyUIMode();

protected:
	void SetupInputComponent() override;

public:
	void OnPausePressed();

	class UInputMappingContext* GetPauseInputMappingContext() const;
	class UInputMappingContext* GetDefaultInputMappingContext() const;

	UFUNCTION()
	void ChangeInputMode(EPlayerInputMode NewInputMode);

	EPlayerInputMode GetCurrentInputMode() const;

};
