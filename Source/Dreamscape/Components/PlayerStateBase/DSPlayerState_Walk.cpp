// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerStateBase/DSPlayerState_Walk.h"
#include "Components/DSPlayerFSMComponent.h"

void UDSPlayerState_Walk::Enter()
{
}

void UDSPlayerState_Walk::Update(float DeltaTime)
{
}

void UDSPlayerState_Walk::Exit()
{
}

void UDSPlayerState_Walk::OnRoll()
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}
	OwnerFSMComponent->ChangeState(EPlayerStateType::EPS_Roll);
}

void UDSPlayerState_Walk::OnSwordAttack()
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	OwnerFSMComponent->ChangeState(EPlayerStateType::EPS_SwordAttack);
}
