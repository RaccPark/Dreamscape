// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DSAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UDSAnimInstance::UDSAnimInstance()
{
	MovingThreshould = 3.0f;
	bIsDead = false;
}

void UDSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		CharacterMovementComponent = Owner->GetCharacterMovement();
	}
}

void UDSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (CharacterMovementComponent)
	{
		Velocity = CharacterMovementComponent->Velocity;
		GroundSpeed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();
	}
}
