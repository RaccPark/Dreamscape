// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerStateBase/DSPlayerState_Hit.h"
#include "Character/DSCharacterPlayer.h"
#include "Components/DSPlayerVitalityComponent.h"

void UDSPlayerState_Hit::Enter()
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		return;
	}

	// 경직 중에는 연속 피격을 받지 않는다.
	if (UDSVitalityComponent* VitalityComponent = PlayerCharacter->GetVitalityComponent())
	{
		VitalityComponent->SetInvincible(true);
	}

	PlayerCharacter->PlayHitMontage();
}

void UDSPlayerState_Hit::Update(float DeltaTime)
{
}

void UDSPlayerState_Hit::Exit()
{
	if (!PlayerCharacter)
	{
		return;
	}

	if (UDSVitalityComponent* VitalityComponent = PlayerCharacter->GetVitalityComponent())
	{
		VitalityComponent->SetInvincible(false);
	}
}

void UDSPlayerState_Hit::OnHit()
{
	UE_LOG(LogTemp, Warning, TEXT("이미 맞고 있습니다!"));
}
