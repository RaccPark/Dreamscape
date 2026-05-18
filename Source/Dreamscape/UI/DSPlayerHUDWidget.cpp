// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DSPlayerHUDWidget.h"
#include "UI/DSResourceBarWidget.h"

void UDSPlayerHUDWidget::NativeConstruct()
{
	if (WBPResourceBar)
	{
		WBPResourceBar->SetVisibility(ESlateVisibility::Hidden);
	}

	PlayAppearTransition();
}

void UDSPlayerHUDWidget::PlayAppearTransition()
{
	if (!WBPResourceBar)
	{
		return;
	}

	WBPResourceBar->SetVisibility(ESlateVisibility::Visible);
	// Play Animation
	// ...
}

void UDSPlayerHUDWidget::PlayDisappearTransition()
{
	if (!WBPResourceBar)
	{
		return;
	}

	// Play Animation
	// ...
}
