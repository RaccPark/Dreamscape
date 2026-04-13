// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerStateBase/DSPlayerState_Hit.h"
#include "Character/DSCharacterPlayer.h"

void UDSPlayerState_Hit::Enter()
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	PlayerCharacter->PlayHitMontage();
}

void UDSPlayerState_Hit::Update(float DeltaTime)
{
}

void UDSPlayerState_Hit::Exit()
{
}

void UDSPlayerState_Hit::OnHit()
{
	UE_LOG(LogTemp, Warning, TEXT("이미 맞고 있습니다!"));
}
