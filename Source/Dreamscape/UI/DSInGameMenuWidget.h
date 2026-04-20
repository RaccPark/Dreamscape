// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DSInGameMenuWidget.generated.h"

/**
 * 
 */
enum class EPauseMenuType : uint8
{
	PMT_Weapon,
	PMT_Collectable,
	PMT_Settings,
};

UCLASS()
class DREAMSCAPE_API UDSInGameMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> WidgetSwitcher;

public:
	void SwitchWidget(EPauseMenuType MenuType);
};
