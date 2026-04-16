// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/DSAnimNotify_RollEnd.h"
#include "Character/DSCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/DSPlayerFSMComponent.h"

void UDSAnimNotify_RollEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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

	CharacterPlayer->OnRollEnd();
}
