// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DSResourceBarWidget.h"

#include "Character/DSCharacterPlayer.h"

#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"

void UDSResourceBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitHPBar();
}

void UDSResourceBarWidget::InitHPBar()
{
	if (!HPBox_HorizontalBox || !HPCell_Texture)
	{
		return;
	}

	HPCells.Empty();
	HPBox_HorizontalBox->ClearChildren();

	// Owner에게서 CurrentHealth와 MaxHealth를 받아오는 구간
	// ...

	for (int32 i = 0; i < CurrentHealth; i++)
	{
		AddHPCell();
	}

	UpdateHPBarBackground();
}

void UDSResourceBarWidget::UpdateHPBarBackground()
{
	if (!HPBoxBackground_Image)
	{
		return;
	}

	if (BackgroundsByMaxHP.Contains(MaxHealth))
	{
		UTexture2D* Texture = BackgroundsByMaxHP[MaxHealth];

		FSlateBrush Brush;
		Brush.SetResourceObject(Texture);

		HPBoxBackground_Image->SetBrush(Brush);
	}
}

void UDSResourceBarWidget::SetMaxHealth(int32 NewMaxHealth)
{
	MaxHealth = NewMaxHealth;

	UpdateHPBarBackground();

	UpdateHealth(CurrentHealth);
}

void UDSResourceBarWidget::AddHPCell()
{
	UImage* NewCell = CreateHPCell();

	HPBox_HorizontalBox->AddChild(NewCell);
	HPCells.Add(NewCell);
}

UImage* UDSResourceBarWidget::CreateHPCell()
{
	UImage* NewCell = NewObject<UImage>(this);

	if (!HPCell_Texture)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UDSResourceBarWidget] There is no HPCell Image!"));
		return nullptr;
	}

	FSlateBrush Brush;
	Brush.SetResourceObject(HPCell_Texture);
	Brush.ImageSize = FVector2D(58.f, 34.f);

	NewCell->SetBrush(Brush);

	return NewCell;
}

void UDSResourceBarWidget::RemoveHPCell()
{
	if (HPCells.Num() == 0)
	{
		return;
	}

	UImage* LastCell = HPCells.Last();

	HPBox_HorizontalBox->RemoveChild(LastCell);
	HPCells.RemoveAt(HPCells.Num() - 1);
}

void UDSResourceBarWidget::UpdateHealth(int32 NewHealth)
{
	CurrentHealth = FMath::Clamp(NewHealth, 0, MaxHealth);

	int32 CurrentCount = HPCells.Num();

	if (CurrentHealth > CurrentCount)
	{
		int32 AddCount = CurrentHealth - CurrentCount;
		for (int32 i = 0; i < AddCount; i++)
		{
			AddHPCell();
		}
	}
	else if (CurrentHealth < CurrentCount)
	{
		int32 RemoveCount = CurrentCount - CurrentHealth;
		for (int32 i = 0; i < RemoveCount; i++)
		{
			RemoveHPCell();
		}
	}
}
