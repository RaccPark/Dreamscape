// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/DSButton.h"
#include "Components/WidgetComponent.h"

#include "Interface/DSActivatableInterface.h"
#include "Character/DSCharacterPlayer.h"
#include "UI/DSInteractoinWidget.h"

// Sets default values
ADSButton::ADSButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ButtonMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMeshComponent"));
	SetRootComponent(ButtonMeshComponent);

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InteractionWidgetComponent->SetupAttachment(RootComponent);
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidgetComponent->SetDrawSize(FVector2D(300.f, 50.f));
	InteractionWidgetComponent->SetVisibility(false);
	InteractionWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 5.f));

	bWasInteracted = false;
	bIsActiveOnlyOnce = false;
}

// Called when the game starts or when spawned
void ADSButton::BeginPlay()
{
	Super::BeginPlay();
	
	if (InteractionWidgetComponent && InteractionWidgetClass)
	{
		InteractionWidgetComponent->SetWidgetClass(InteractionWidgetClass);

		InteractionWidget = Cast<UDSInteractoinWidget>(InteractionWidgetComponent->GetUserWidgetObject());
		if (InteractionWidget)
		{
			InteractionWidget->SetPromptText(FText::FromString(ButtonTip));
			InteractionWidget->SetBindKeyText(FText::FromString("E"));
		}
	}
}

// Called every frame
void ADSButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADSButton::Interact(ADSCharacterPlayer* Player)
{
	UE_LOG(LogTemp, Log, TEXT("Button Interacted!"));
	if (!LinkedActors.IsEmpty() && !bWasInteracted)
	{
		for (AActor* LinkedActor : LinkedActors)
		{
			IDSActivatableInterface* ActivatableActor = Cast<IDSActivatableInterface>(LinkedActor);

			if (ActivatableActor)
			{
				UE_LOG(LogTemp, Log, TEXT("Activating linked actor: %s"), *LinkedActor->GetName());
				ActivatableActor->Activate();
			}
		}

		if (bIsActiveOnlyOnce)
		{
			bWasInteracted = true;

			if (InteractionWidgetComponent)
			{
				InteractionWidgetComponent->SetVisibility(false);
			}
		}
	}
}

void ADSButton::OnFocus()
{
	UE_LOG(LogTemp, Log, TEXT("Button Focused!"));
	if (InteractionWidgetComponent && !bWasInteracted)
	{
		InteractionWidgetComponent->SetVisibility(true);
	}
}

void ADSButton::OnFocusLost()
{
	if (InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetVisibility(false);
	}
}

