// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/DSAnimNotify_FallEnd.h"
#include "Character/DSCharacterPlayer.h"
#include "Components/DSPlayerFSMComponent.h"

void UDSAnimNotify_FallEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	if (!MeshComp)
	{
		return;
	}

	ADSCharacterPlayer* CharacterPlayer = Cast<ADSCharacterPlayer>(MeshComp->GetOwner());
	if (!CharacterPlayer)
	{
		return;
	}

	UDSPlayerFSMComponent* PlayerFSM = CharacterPlayer->FindComponentByClass<UDSPlayerFSMComponent>();
	if (!PlayerFSM)
	{
		return;
	}
	PlayerFSM->PopPlayerState();
}
