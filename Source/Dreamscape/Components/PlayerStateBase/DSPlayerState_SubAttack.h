// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerStateBase/DSPlayerStateBase.h"
#include "DSPlayerState_SubAttack.generated.h"

/**
 * 서브 공격 방식은 아래와 같다
 * 서브 공격 발동(우클릭) -> 충전 -> 발동 및 취소(우클릭 해제) -> 서브 공격 종료 -> 회복 -> Idle
 */
UCLASS()
class DREAMSCAPE_API UDSPlayerState_SubAttack : public UDSPlayerStateBase
{
	GENERATED_BODY()
	
public:
	virtual void Enter() override;
	virtual void Exit() override;

	virtual void OnSubAttackReleased() override;
	virtual void OnSubAttackEnded() override;
};
