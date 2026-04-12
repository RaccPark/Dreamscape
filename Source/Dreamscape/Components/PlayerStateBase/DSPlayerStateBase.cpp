// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerStateBase/DSPlayerStateBase.h"
#include "Components/DSPlayerFSMComponent.h"
#include "Character/DSCharacterPlayer.h"

void UDSPlayerStateBase::Initialize(UDSPlayerFSMComponent* PlayerFSMComponent)
{
	OwnerFSMComponent = PlayerFSMComponent;

	if (OwnerFSMComponent)
	{
		PlayerCharacter = Cast<ADSCharacterPlayer>(OwnerFSMComponent->GetOwner());
	}
}

void UDSPlayerStateBase::Enter()
{
	UE_LOG(LogTemp, Log, TEXT("Entering state: %s"), *GetClass()->GetName());
}

void UDSPlayerStateBase::Update(float DeltaTime)
{

}

void UDSPlayerStateBase::Exit()
{
	UE_LOG(LogTemp, Log, TEXT("Exiting state: %s"), *GetClass()->GetName());
}

void UDSPlayerStateBase::OnMove(const struct FInputActionValue& Value)
{

}

void UDSPlayerStateBase::OnRoll()
{
	UE_LOG(LogTemp, Log, TEXT("Roll input received in state: %s"), *GetClass()->GetName());
}

void UDSPlayerStateBase::OnSwordAttack()
{
	UE_LOG(LogTemp, Log, TEXT("Sword attack input received in state: %s"), *GetClass()->GetName());
}

void UDSPlayerStateBase::OnComboActionEnd()
{

}

void UDSPlayerStateBase::OnDeath()
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	OwnerFSMComponent->ChangeState(EPlayerStateType::EPS_Death);
}
