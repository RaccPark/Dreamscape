// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DSAnimNotifyState_Attack.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSAnimNotifyState_Attack : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
