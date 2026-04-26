// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DSInteractoinWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSInteractoinWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Prompt_Text;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BindKey_Text;

	void SetPromptText(const class FText& NewText);
	void SetBindKeyText(const class FText& NewText);
};
