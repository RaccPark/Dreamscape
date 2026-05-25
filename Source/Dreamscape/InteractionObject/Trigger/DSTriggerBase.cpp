// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/Trigger/DSTriggerBase.h"
#include "Components/BoxComponent.h"

// Sets default values
ADSTriggerBase::ADSTriggerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));

	bTriggerOnce = true;
	bAlreadyTriggered = false;
}

// Called when the game starts or when spawned
void ADSTriggerBase::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADSTriggerBase::OnOverlapBegin);
}

void ADSTriggerBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bAlreadyTriggered)
	{
		UE_LOG(LogTemp, Log, TEXT("Trigger already activated, ignoring overlap with: %s"), *OtherActor->GetName());
		return;
	}

	if (!OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap with null actor, ignoring."));
		return;
	}

	// 특정 타입만 허용
	if (!AllowedActorClass || !OtherActor->IsA(AllowedActorClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor %s is not of allowed class %s, ignoring."), *OtherActor->GetName(), *AllowedActorClass->GetName());
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Trigger activated by: %s"), *OtherActor->GetName());
	bAlreadyTriggered = true;

	OnTriggerActivatedDelegate.Broadcast(OtherActor);
}

// Called every frame
void ADSTriggerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

