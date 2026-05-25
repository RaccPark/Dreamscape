// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerStateBase/DSPlayerState_SwordAttack.h"
#include "Character/DSCharacterPlayer.h"
#include "Components/DSPlayerFSMComponent.h"

void UDSPlayerState_SwordAttack::Enter()
{
	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_SwordAttack] Entering SwordAttack state."));

	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	if (!bIsAttacking)
	{
		bIsAttacking = true;
		PlayerCharacter->SetEquippedWeaponSocket("RightWeaponEquipSocket");
	}

	PlayerCharacter->RotateCharacterToMouseCursor();
	PlayerCharacter->ProcessComboCommand();
}
void UDSPlayerState_SwordAttack::Update(float DeltaTime)
{

}

void UDSPlayerState_SwordAttack::Exit()
{

	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_SwordAttack] Exiting SwordAttack state."));
}

void UDSPlayerState_SwordAttack::OnSwordAttack()
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	PlayerCharacter->ProcessComboCommand();
}

void UDSPlayerState_SwordAttack::OnComboActionEnd()
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	bIsAttacking = false;
	PlayerCharacter->SetEquippedWeaponSocket("WeaponIdleSocket");
	OwnerFSMComponent->ChangeState(EPlayerStateType::EPS_Idle);
}
