// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/DSAnimNotify_HitEnd.h"
#include "Components/DSPlayerFSMComponent.h"

void UDSAnimNotify_HitEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* OwnerCharacter = MeshComp->GetOwner();
	UDSPlayerFSMComponent* PlayerFSMComponent = OwnerCharacter->FindComponentByClass<UDSPlayerFSMComponent>();
	if (PlayerFSMComponent)
	{
		if (PlayerFSMComponent->GetCurrentStateType() == EPlayerStateType::EPS_Hit)
		{
			PlayerFSMComponent->ChangeState(EPlayerStateType::EPS_Idle);
		}
		
	}
}
