// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DSResourceBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSResourceBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> HPBoxBackground_Image;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> HPBox_HorizontalBox;

	// HP Cell Texture
	// HorizontalBox 적용 시 Image Size는 58 x 34로 적용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UTexture2D> HPCell_Texture;

	// HP Box Background Image
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TMap<int32, class UTexture2D*> BackgroundsByMaxHP;

	// 현재 생성된 HP Cell
	UPROPERTY()
	TArray<TObjectPtr<class UImage>> HPCells;

	int32 CurrentHealth = 4;
	int32 MaxHealth = 4;

	void InitHPBar();
	void AddHPCell();
	UImage* CreateHPCell();
	void RemoveHPCell();


public:
	void BindToPlayer(class ADSCharacterPlayer* Player);

	// UDSVitalityComponent::OnHealthChanged 구독용.
	// 플레이어 체력은 정수 칸이지만 컴포넌트가 float로 들고 있어 여기서 int32로 바꾼다.
	UFUNCTION()
	void HandleHealthChanged(float NewHealth, float NewMaxHealth);

	UFUNCTION()
	void UpdateHealth(int32 NewHealth);

	void UpdateHPBarBackground();
	void SetMaxHealth(int32 NewMaxHealth);
};
