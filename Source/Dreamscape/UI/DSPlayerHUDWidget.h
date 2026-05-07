// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DSPlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UDSResourceBarWidget> WBPResourceBar;
};
