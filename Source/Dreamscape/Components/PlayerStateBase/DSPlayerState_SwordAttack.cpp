// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerStateBase/DSPlayerState_SwordAttack.h"
#include "Character/DSCharacterPlayer.h"
#include "Components/DSPlayerFSMComponent.h"
#include "Components/DSPlayerCombatComponent.h"

void UDSPlayerState_SwordAttack::Enter()
{
	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_SwordAttack] Entering SwordAttack state."));

	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	UDSPlayerCombatComponent* CombatComponent = PlayerCharacter->GetCombatComponent();
	if (!CombatComponent)
	{
		return;
	}

	if (!bIsAttacking)
	{
		bIsAttacking = true;
		CombatComponent->SetWeaponSocket("RightWeaponEquipSocket");
	}

	PlayerCharacter->RotateCharacterToMouseCursor();
	CombatComponent->ProcessComboCommand();
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

	// 회복 중이면 CombatComponent가 무시한다. 여기서는 그냥 넘긴다.
	if (UDSPlayerCombatComponent* CombatComponent = PlayerCharacter->GetCombatComponent())
	{
		CombatComponent->ProcessComboCommand();
	}
}

void UDSPlayerState_SwordAttack::OnComboActionEnd()
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	// 회복 시간까지 끝난 뒤에 불린다 (CombatComponent::OnAttackSequenceEnded → 캐릭터 → FSM)
	bIsAttacking = false;

	if (UDSPlayerCombatComponent* CombatComponent = PlayerCharacter->GetCombatComponent())
	{
		CombatComponent->SetWeaponSocket("WeaponIdleSocket");
	}

	OwnerFSMComponent->ChangeState(EPlayerStateType::EPS_Idle);
}
