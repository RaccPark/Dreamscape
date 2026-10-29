// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerStateBase/DSPlayerState_SubAttack.h"
#include "Components/DSPlayerFSMComponent.h"
#include "Character/DSCharacterPlayer.h"

void UDSPlayerState_SubAttack::Enter()
{
	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_SubAttack] SubAttack State Entered"));

	// 향후 서브 공격 충전 로직을 구현할 예정
}

void UDSPlayerState_SubAttack::Exit()
{
	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_SubAttack] SubAttack State Exited"));

	// 향후 충전 취소
	// 피격 등으로 상태가 끊길 때 충전 타이머 정리
}

void UDSPlayerState_SubAttack::OnSubAttackReleased()
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_SubAttack] PlayerCharacter or OwnerFSMComponent is null"));
		return;
	}

	// 발동 방향 -> 마우스 버튼을 뗀 커서 방향
	PlayerCharacter->RotateCharacterToMouseCursor();

	// Release()
}

void UDSPlayerState_SubAttack::OnSubAttackEnded()
{
	if (!PlayerCharacter || !OwnerFSMComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerState_SubAttack] PlayerCharacter or OwnerFSMComponent is null"));
		return;
	}
	
	// 회복 시간까지 끝난 뒤에 호출(OnSubAbilityEnded -> 캐릭터 -> FSM)
	OwnerFSMComponent->ChangeState(EPlayerStateType::EPS_Idle);
}
