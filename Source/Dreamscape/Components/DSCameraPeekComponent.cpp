// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DSCameraPeekComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UDSCameraPeekComponent::UDSCameraPeekComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Initialize status
	bIsPeeking = false;

	// Initialize config
	MaxPeekDistance = 400.0f;
	InterpSpeed = 6.0f;
	DeadZone = 20.0f;
	MaxInputRange = 300.0f;
}

void UDSCameraPeekComponent::SetPeekActive(bool bActive)
{
	bIsPeeking = bActive;
	if (!bIsPeeking)
	{
		AccumulatedMouse = FVector2D::ZeroVector;
	}
}

void UDSCameraPeekComponent::AddMouseDelta(const FVector2D& Delta)
{
	if (bIsPeeking)
	{
		// Peek 방향 유지를 위해 누적
		AccumulatedMouse += Delta;
	}
}


// Called when the game starts
void UDSCameraPeekComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("DSCameraPeekComponent: Owner is null"));
		return;
	}
	
	SpringArmComponent = Owner->FindComponentByClass<USpringArmComponent>();
	CameraComponent = Owner->FindComponentByClass<UCameraComponent>();
}


// Called every frame
void UDSCameraPeekComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!SpringArmComponent || !CameraComponent)
	{
		return;
	}

	UpdatePeekOffset(DeltaTime);
}

void UDSCameraPeekComponent::UpdatePeekOffset(float DeltaTime)
{
	FVector TargetOffset = FVector::ZeroVector;

	if (bIsPeeking)
	{
		FVector2D Input = AccumulatedMouse;
		const float InputSize = Input.Size();

		// InputSize > DeadZone은 마우스가 일정 입력 지점을 넘어서면 작동하게 설계
		if (InputSize > DeadZone)
		{
			// Direction
			FVector2D Dir = Input / InputSize;

			// 선형 입력은 부자연스러우므로, 비선형 감도 적용
			float Alpha = FMath::Clamp(InputSize / MaxInputRange, 0.0f, 1.0f);
			Alpha = FMath::Pow(Alpha, 1.5f);

			// 상하 피크 시 카메라의 높이 변경 없이 피킹 가능하게 제한(카메라가 아래로 뚫고 내려가는 현상 방지)
			FVector FlatForward = CameraComponent->GetForwardVector();
			FlatForward.Z = 0.0f;
			FlatForward.Normalize();

			// Screen space to World space
			// 카메라가 회전된 상태에서도 Peek 방향이 항상 화면 기준으로 유지
			FVector WorldDir = CameraComponent->GetRightVector() * Dir.X + FlatForward * Dir.Y;

			TargetOffset = WorldDir * MaxPeekDistance * Alpha;
		}
	}

	// Interp
	// 목표로 즉시 이동하지 않고 보간
	// Peek 동작/해제 시 부드럽게 이동
	CurrentOffset = FMath::VInterpTo(CurrentOffset, TargetOffset, DeltaTime, InterpSpeed);

	SpringArmComponent->TargetOffset = CurrentOffset;
}

