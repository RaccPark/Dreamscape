// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerStateBase/DSPlayerState_Idle.h"
#include "Character/DSCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/DSPlayerFSMComponent.h"

void UDSPlayerState_Idle::OnRoll()
{
	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_Idle] OnRoll called. Attempting to transition to Roll state."));
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	OwnerFSMComponent->ChangeState(EPlayerStateType::EPS_Roll);
	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_Idle] Transitioned to Roll state."));
}
