// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DSAnimInstance.h"
#include "Character/DSCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UDSAnimInstance::UDSAnimInstance()
{
	MovingThreshould = 3.0f;
	bIsDead = false;
}

void UDSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ADSCharacterPlayer>(GetOwningActor());
	if (Owner)
	{
		CharacterMovementComponent = Owner->GetCharacterMovement();
		Owner->OnDeathDelegate.AddUObject(this, &UDSAnimInstance::OnOwnerDeath);
	}
}

void UDSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (bIsDead)
	{
		return;
	}

	if (CharacterMovementComponent)
	{
		Velocity = CharacterMovementComponent->Velocity;
		GroundSpeed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();
	}
}

void UDSAnimInstance::OnOwnerDeath()
{
	bIsDead = true;
}
