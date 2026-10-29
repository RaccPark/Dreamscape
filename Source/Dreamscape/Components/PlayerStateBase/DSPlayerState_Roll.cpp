// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerStateBase/DSPlayerState_Roll.h"
#include "Character/DSCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/DSPlayerFSMComponent.h"
#include "Components/DSPlayerVitalityComponent.h"
#include "Animation/AnimInstance.h"

void UDSPlayerState_Roll::Enter()
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_Roll] Entering Roll state."));

	//PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();

	// 구르는 동안은 무적. 피해도 넉백도 들어가지 않는다.
	if (UDSVitalityComponent* VitalityComponent = PlayerCharacter->GetVitalityComponent())
	{
		VitalityComponent->SetInvincible(true);
	}

	PlayerCharacter->PlayRollMontage();

}

void UDSPlayerState_Roll::Update(float DeltaTime)
{
}

void UDSPlayerState_Roll::Exit()
{
	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_Roll] Exiting Roll state."));

	if (PlayerCharacter)
	{
		if (UDSVitalityComponent* VitalityComponent = PlayerCharacter->GetVitalityComponent())
		{
			VitalityComponent->SetInvincible(false);
		}
	}
}

void UDSPlayerState_Roll::OnRollEnd()
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	bool bIsFalling = PlayerCharacter->GetCharacterMovement()->IsFalling();
	if (bIsFalling)
	{
		OwnerFSMComponent->PopPlayerState();
		OwnerFSMComponent->PushPlayerState(EPlayerStateType::EPS_Fall);
	}
	else
	{
		OwnerFSMComponent->PopPlayerState();
	}
}

void UDSPlayerState_Roll::OnFall()
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	OwnerFSMComponent->ChangeState(EPlayerStateType::EPS_Fall);
}

void UDSPlayerState_Roll::OnHit()
{
	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_Roll] 회피"));
}

 