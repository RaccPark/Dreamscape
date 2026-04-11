// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/DSAnimNotifyState_Melee.h"
#include "Character/Enemy/DSEnemyMelee.h"

void UDSAnimNotifyState_Melee::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		// 애니메이션을 실행 중인 주인이 ADSEnemyMelee 인지 확인
		ADSEnemyMelee* Enemy = Cast<ADSEnemyMelee>(MeshComp->GetOwner());
		if (Enemy)
		{
			Enemy->StartMeleeTrace();
		}
	}
}

void UDSAnimNotifyState_Melee::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ADSEnemyMelee* Enemy = Cast<ADSEnemyMelee>(MeshComp->GetOwner());
		if (Enemy)
		{
			Enemy->PerformMeleeTrace();
		}
	}
}

void UDSAnimNotifyState_Melee::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ADSEnemyMelee* Enemy = Cast<ADSEnemyMelee>(MeshComp->GetOwner());
		if (Enemy)
		{
			Enemy->EndMeleeTrace();
		}
	}
}
