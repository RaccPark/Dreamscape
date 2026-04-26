// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DSInteractionComponent.h"
#include "Interface/DSInteractableInterface.h"
#include "Character/DSCharacterPlayer.h"

// Sets default values for this component's properties
UDSInteractionComponent::UDSInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InteractionRange = 75.f;
	DetectionRadius = 100.f;
}


// Called when the game starts
void UDSInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UDSInteractionComponent::TryInteract()
{
	if (!CurrentActor)
	{
		return;
	}

	ExecuteInteraction();

	/*
	float Distance = FVector::Distance(GetOwner()->GetActorLocation(), CurrentActor->GetActorLocation());

	if (Distance > InteractionRange)
	{
		// 멀리서 인터렉션의 경우 그 위치로 이동할 수 있게
		MoveToTarget();
	}
	else
	{
		ExecuteInteraction();
	}
	*/
}

void UDSInteractionComponent::MoveToTarget()
{
	UE_LOG(LogTemp, Warning, TEXT("Move to Target: %s"), *CurrentActor->GetName());
}

void UDSInteractionComponent::ExecuteInteraction()
{
	UE_LOG(LogTemp, Warning, TEXT("Execute Interaction: %s"), *CurrentActor->GetName());

	if (CurrentInteractable)
	{
		CurrentInteractable->Interact(Cast<ADSCharacterPlayer>(GetOwner()));
	}
}

void UDSInteractionComponent::UpdateInteractable()
{
	FVector Start = GetOwner()->GetActorLocation();

	TArray<FOverlapResult> Results;

	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(DetectionRadius);

	GetWorld()->OverlapMultiByChannel(Results, Start, FQuat::Identity, ECC_Visibility, CollisionSphere);

	AActor* ClosetActor = nullptr;
	float ClosestDistance = MAX_FLT;	// 오옹이 최대값 디파인이 있구나

	for (auto& Result : Results)
	{
		AActor* Actor = Result.GetActor();
		if (!Actor)
		{
			continue;
		}

		if (!Actor->GetClass()->ImplementsInterface(UDSInteractableInterface::StaticClass()))
		{
			continue;
		}

		float Distance = FVector::Distance(Start, Actor->GetActorLocation());

		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosetActor = Actor;
		}

	}

	SetCurrentActor(ClosetActor);
}

void UDSInteractionComponent::SetCurrentActor(AActor* NewActor)
{
	if (CurrentActor == NewActor)
	{
		return;
	}

	if (CurrentInteractable)
	{
		CurrentInteractable->OnFocusLost();
		//CurrentInteractable = nullptr;
	}

	CurrentActor = NewActor;

	if (NewActor && NewActor->GetClass()->ImplementsInterface(UDSInteractableInterface::StaticClass()))
	{
		CurrentInteractable = TScriptInterface<IDSInteractableInterface>(NewActor);

		// UI
		if (CurrentInteractable)
		{
			CurrentInteractable->OnFocus();
		}
	}
	else
	{
		CurrentInteractable = nullptr;
	}
}

// Called every frame
void UDSInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateInteractable();
}

