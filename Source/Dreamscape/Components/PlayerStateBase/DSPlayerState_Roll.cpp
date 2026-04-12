// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerStateBase/DSPlayerState_Roll.h"
#include "Character/DSCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/DSPlayerFSMComponent.h"
#include "Animation/AnimInstance.h"

void UDSPlayerState_Roll::Enter()
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_Roll] Entering Roll state."));

	//PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();
	
	PlayerCharacter->PlayRollMontage();
	
}

void UDSPlayerState_Roll::Update(float DeltaTime)
{
}

void UDSPlayerState_Roll::Exit()
{
	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_Roll] Exiting Roll state."));
}

 