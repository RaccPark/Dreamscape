// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DSInGameMenuWidget.h"
#include "Components/WidgetSwitcher.h"

void UDSInGameMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (WidgetSwitcher)
	{
		SwitchWidget(EPauseMenuType::PMT_Weapon);
	}
}

void UDSInGameMenuWidget::SwitchWidget(EPauseMenuType MenuType)
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(static_cast<int32>(MenuType));
	}
}
