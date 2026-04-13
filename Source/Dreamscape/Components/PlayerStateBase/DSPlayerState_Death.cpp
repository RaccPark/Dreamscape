// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerStateBase/DSPlayerState_Death.h"
#include "Character/DSCharacterPlayer.h"

void UDSPlayerState_Death::Enter()
{
	if (!PlayerCharacter)
	{
		return;
	}

	PlayerCharacter->OnDeath();
}

void UDSPlayerState_Death::Update(float DeltaTime)
{

}

void UDSPlayerState_Death::Exit()
{

}

void UDSPlayerState_Death::OnHit()
{
	UE_LOG(LogTemp, Warning, TEXT("이미 죽음."));
}
