// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/DSPickableItem.h"
#include "Components/WidgetComponent.h"
#include "UI/DSInteractoinWidget.h"
#include "Character/DSCharacterPlayer.h"
#include "Components/Inventory/DSItemData.h"
#include "Components/DSInventoryComponent.h"

// Sets default values
ADSPickableItem::ADSPickableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	SetRootComponent(ItemMeshComponent);

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InteractionWidgetComponent->SetupAttachment(RootComponent);
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidgetComponent->SetDrawSize(FVector2D(300.f, 50.f));
	InteractionWidgetComponent->SetVisibility(false);
	InteractionWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 5.f));
}

// Called when the game starts or when spawned
void ADSPickableItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (InteractionWidgetComponent && InteractionWidgetClass)
	{
		InteractionWidgetComponent->SetWidgetClass(InteractionWidgetClass);

		InteractionWidget = Cast<UDSInteractoinWidget>(InteractionWidgetComponent->GetUserWidgetObject());
		if (InteractionWidget)
		{
			InteractionWidget->SetPromptText(FText::FromString("Pick Up"));
			InteractionWidget->SetBindKeyText(FText::FromString("E"));
		}
	}
}

// Called every frame
void ADSPickableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADSPickableItem::Interact(ADSCharacterPlayer* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("Picked up: %s"), *GetName());

	if (!Player || !ItemData)
	{
		return;
	}

	Player->GetInventoryComponent()->AddItem(ItemData, 1);

	Destroy();

}

void ADSPickableItem::OnFocus()
{
	if (InteractionWidgetComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Focused on: %s"), *GetName());
		InteractionWidgetComponent->SetVisibility(true);
	}
}

void ADSPickableItem::OnFocusLost()
{
	if (InteractionWidgetComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Focus lost on: %s"), *GetName());
		InteractionWidgetComponent->SetVisibility(false);
	}
}

