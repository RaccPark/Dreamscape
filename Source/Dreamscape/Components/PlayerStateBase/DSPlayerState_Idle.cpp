// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerStateBase/DSPlayerState_Idle.h"
#include "Character/DSCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/DSPlayerFSMComponent.h"

void UDSPlayerState_Idle::Enter()
{
}

void UDSPlayerState_Idle::Update(float DeltaTime)
{
}

void UDSPlayerState_Idle::Exit()
{
}

void UDSPlayerState_Idle::OnMove(const struct FInputActionValue& Value)
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	OwnerFSMComponent->ChangeState(EPlayerStateType::EPS_Walk);
}

void UDSPlayerState_Idle::OnRoll()
{
	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_Idle] OnRoll called. Attempting to transition to Roll state."));
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	OwnerFSMComponent->PushPlayerState(EPlayerStateType::EPS_Roll);
	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_Idle] Transitioned to Roll state."));
}

void UDSPlayerState_Idle::OnFall()
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}
	
	OwnerFSMComponent->ChangeState(EPlayerStateType::EPS_Fall);
}

void UDSPlayerState_Idle::OnSwordAttack()
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	OwnerFSMComponent->ChangeState(EPlayerStateType::EPS_SwordAttack);
}

void UDSPlayerState_Idle::OnSubAttackPressed()
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	// 향후 구현 필요한 부분
	// SubAbilityComponent->CanBeginCharge()가 false면 전이하지 않음(빈 슬롯, 회복 중, 스테미나 부족)
	OwnerFSMComponent->ChangeState(EPlayerStateType::EPS_SubAttack);
}
