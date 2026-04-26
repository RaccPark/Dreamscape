// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DSInteractoinWidget.h"
#include "DSInteractoinWidget.h"
#include "Components/TextBlock.h"

void UDSInteractoinWidget::SetPromptText(const FText& NewText)
{
	if (!Prompt_Text)
	{
		return;
	}
	Prompt_Text->SetText(NewText);
}

void UDSInteractoinWidget::SetBindKeyText(const FText& NewText)
{
	if (!BindKey_Text)
	{
		return;
	}
	BindKey_Text->SetText(NewText);
}
