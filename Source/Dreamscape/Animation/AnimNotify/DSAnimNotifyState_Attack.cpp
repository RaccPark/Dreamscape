// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/DSAnimNotifyState_Attack.h"
#include "Character/DSCharacterPlayer.h"
#include "Weapon/DSSwordWeaponBase.h"
#include "Interface/DSAttackTraceInterface.h"

void UDSAnimNotifyState_Attack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
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

	IDSAttackTraceInterface* AttackTraceInterface = Cast<IDSAttackTraceInterface>(Owner);
	if (!AttackTraceInterface)
	{
		return;
	}

	// 공격 트레이스 시작
	AttackTraceInterface->StartAttackTrace();
}

void UDSAnimNotifyState_Attack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
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

	IDSAttackTraceInterface* AttackTraceInterface = Cast<IDSAttackTraceInterface>(Owner);
	if (!AttackTraceInterface)
	{
		return;
	}

	// 공격 트레이스 틱
	AttackTraceInterface->PerformAttackTrace();
}



void UDSAnimNotifyState_Attack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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

	IDSAttackTraceInterface* AttackTraceInterface = Cast<IDSAttackTraceInterface>(Owner);
	if (!AttackTraceInterface)
	{
		return;
	}

	// 공격 트레이스 틱
	AttackTraceInterface->EndAttackTrace();
}
