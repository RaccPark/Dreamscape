// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DSCollectableSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSCollectableSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> CollectableSlot_Background_Image;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> CollectableSlot_Collectable_Image;

};
