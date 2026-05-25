// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/DSAnimNotify_AttackEnd.h"
#include "Interface/DSWeaponAttachInterface.h"
#include "Character/Enemy/DSBoss01.h"

void UDSAnimNotify_AttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
	{
		return;
	}
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	IDSWeaponAttachInterface* WeaponAttachInterface = Cast<IDSWeaponAttachInterface>(Owner);
	if (WeaponAttachInterface)
	{
		WeaponAttachInterface->AttachWeaponBySocket(Boss01Socket::IdleSocket);
	}
	
}
