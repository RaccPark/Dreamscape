// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/DSDoor.h"

// Sets default values
ADSDoor::ADSDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(Root);

	bIsOpen = false;
	bIsMoving = false;
}

// Called when the game starts or when spawned
void ADSDoor::BeginPlay()
{
	Super::BeginPlay();
	
	if (DoorCurve)
	{
		FOnTimelineFloat UpdateFunction;
		UpdateFunction.BindUFunction(this, FName("UpdateDoorRotation"));

		DoorTimeline.AddInterpFloat(DoorCurve, UpdateFunction);

		FOnTimelineEvent TimelineFinishedFunction;
		TimelineFinishedFunction.BindUFunction(this, FName("OnTimelineFinished"));

		DoorTimeline.SetTimelineFinishedFunc(TimelineFinishedFunction);
	}

	ClosedRotation = DoorMesh->GetRelativeRotation();

	if (bIsOpen)
	{
		DoorMesh->SetRelativeRotation(OpenRotation);
	}
}

void ADSDoor::UpdateDoorRotation(float Alpha)
{
	FRotator NewRotation = FMath::Lerp(ClosedRotation, OpenRotation, Alpha);
	DoorMesh->SetRelativeRotation(NewRotation);
}

void ADSDoor::OnTimelineFinished()
{
	bIsMoving = false;
}

// Called every frame
void ADSDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DoorTimeline.TickTimeline(DeltaTime);
}

void ADSDoor::Activate()
{
	if (bIsMoving)
	{
		return;
	}

	bIsMoving = true;

	if (bIsOpen)
	{
		DoorTimeline.Reverse();
	}
	else
	{
		DoorTimeline.Play();
	}

	bIsOpen = !bIsOpen;
}

