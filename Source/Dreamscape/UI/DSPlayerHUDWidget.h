// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/DSWidgetTransitionable.h"
#include "DSPlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSPlayerHUDWidget : public UUserWidget, public IDSWidgetTransitionable
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UDSResourceBarWidget> WBPResourceBar;

	virtual void PlayAppearTransition() override;
	virtual void PlayDisappearTransition() override;
};
