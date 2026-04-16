// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DSAnimInstance.h"
#include "Character/DSCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/DSPlayerFSMComponent.h"

UDSAnimInstance::UDSAnimInstance()
{
	MovingThreshould = 3.0f;
	bIsDead = false;
	bIsFalling = false;
}

void UDSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ADSCharacterPlayer>(GetOwningActor());
	if (Owner)
	{
		CharacterMovementComponent = Owner->GetCharacterMovement();
		Owner->OnDeathDelegate.AddUObject(this, &UDSAnimInstance::OnOwnerDeath);
		UDSPlayerFSMComponent* PlayerFSMComponent = Owner->GetPlayerFSMComponent();
		if (PlayerFSMComponent)
		{
			PlayerFSMComponent->OnStateChangedDelegate.AddDynamic(this, &UDSAnimInstance::HandleStateChanged);
		}
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
		// Velocity
		Velocity = CharacterMovementComponent->Velocity;
		GroundSpeed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();

		// Falling
		bIsFalling = CharacterMovementComponent->IsFalling();
	}
}

void UDSAnimInstance::OnOwnerDeath()
{
	bIsDead = true;
}

void UDSAnimInstance::HandleStateChanged(EPlayerStateType NewState)
{
	CurrentState = NewState;
}
