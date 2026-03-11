// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerStateBase/DSPlayerState_SwordAttack.h"
#include "Character/DSCharacterPlayer.h"

void UDSPlayerState_SwordAttack::Enter()
{
	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_SwordAttack] Entering SwordAttack state."));

	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	PlayerCharacter->ProcessComboCommand();
}
void UDSPlayerState_SwordAttack::Exit()
{
	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_SwordAttack] Exiting SwordAttack state."));
}
