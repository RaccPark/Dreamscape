// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/DSAnimNotifyState_Attack.h"
#include "Character/DSCharacterPlayer.h"
#include "Weapon/DSSwordWeapon.h"

void UDSAnimNotifyState_Attack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ADSCharacterPlayer* Player = Cast<ADSCharacterPlayer>(MeshComp->GetOwner());
	if (Player)
	{
		if (ADSSwordWeapon* Weapon = Player->GetEquippedSwordWeapon())
		{
			Weapon->StartAttackTrace();
			// 임시 캐릭터 런치
			Player->LaunchCharacter(Player->GetActorForwardVector() * 600.f, true, true);
		}
	}
}

void UDSAnimNotifyState_Attack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ADSCharacterPlayer* Player = Cast<ADSCharacterPlayer>(MeshComp->GetOwner());
	if (Player)
	{
		if (ADSSwordWeapon* Weapon = Player->GetEquippedSwordWeapon())
		{
			Weapon->EndAttackTrace();
		}
	}
}
