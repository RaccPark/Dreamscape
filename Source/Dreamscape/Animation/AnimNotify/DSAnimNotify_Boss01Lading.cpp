// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/DSAnimNotify_Boss01Lading.h"
#include "Character/Enemy/DSBossCharacterBase.h"

void UDSAnimNotify_Boss01Lading::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp)
	{
		return;
	}

	ADSBossCharacterBase* Boss = Cast<ADSBossCharacterBase>(MeshComp->GetOwner());
	if (Boss)
	{
		Boss->OnBossLandingFinishedDelegate.Broadcast();
	}
}
