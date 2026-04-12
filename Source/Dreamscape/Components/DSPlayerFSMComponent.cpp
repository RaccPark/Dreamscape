// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DSPlayerFSMComponent.h"
#include "Components/PlayerStateBase/DSPlayerStateBase.h"
#include "Character/DSCharacterPlayer.h"

#include "Components/PlayerStateBase/DSPlayerState_Idle.h"
#include "Components/PlayerStateBase/DSPlayerState_Walk.h"
#include "Components/PlayerStateBase/DSPlayerState_Roll.h"
#include "Components/PlayerStateBase/DSPlayerState_SwordAttack.h"
#include "Components/PlayerStateBase/DSPlayerState_Death.h"

// Sets default values for this component's properties
UDSPlayerFSMComponent::UDSPlayerFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

UDSPlayerStateBase* UDSPlayerFSMComponent::GetState(EPlayerStateType StateType) const
{
	if (PlayerStates.Contains(StateType))
	{
		return PlayerStates[StateType];
	}
	UE_LOG(LogTemp, Warning, TEXT("State of type %d not found!"), static_cast<uint8>(StateType));
	return nullptr;
}

void UDSPlayerFSMComponent::ChangeState(EPlayerStateType NewType)
{
	// 상태 변경을 위해 기존 상태 탈출
	// 같은 상태로 변경되는 경우를 대비하는 코드 추후 작성
	if (CurrentState)
	{
		CurrentState->Exit();
	}

	// 이전 상태 저장
	PreviousStateType = CurrentStateType;

	// 새로운 상태 진입
	CurrentStateType = NewType;
	CurrentState = GetState(NewType);

	if (CurrentState)
	{
		CurrentState->Enter();
	}

}

void UDSPlayerFSMComponent::HandleMoveInput(const struct FInputActionValue& Value)
{
	if (CurrentState)
	{
		CurrentState->OnMove(Value);
	}
}

void UDSPlayerFSMComponent::HandleRollInput()
{
	UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerFSMComponent] Roll input received in FSM Component. Current state: %s"), *CurrentState->GetClass()->GetName());

	if (CurrentState)
	{
		if (CurrentStateType == EPlayerStateType::EPS_Roll)
		{
			UE_LOG(LogTemp, Warning, TEXT("[UDSPlayerFSMComponent] Already in Roll state. Ignoring roll input."));
			return;
		}

		CurrentState->OnRoll();
	}
}

void UDSPlayerFSMComponent::HandleSwordAttackInput()
{
	if (CurrentState)
	{
		CurrentState->OnSwordAttack();
	}
}

void UDSPlayerFSMComponent::HandleComboActionEnd()
{
	if (CurrentState)
	{
		CurrentState->OnComboActionEnd();
	}
}

void UDSPlayerFSMComponent::HandleDeath()
{
	if (CurrentState)
	{
		CurrentState->OnDeath();
	}
}

EPlayerStateType UDSPlayerFSMComponent::GetCurrentStateType() const
{
	return CurrentStateType;
}

EPlayerStateType UDSPlayerFSMComponent::GetPreviousStateType() const
{
	return PreviousStateType;
}


// Called when the game starts
void UDSPlayerFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ADSCharacterPlayer>(GetOwner());
	
	// State Class 생성
	CreateState<UDSPlayerState_Idle>(EPlayerStateType::EPS_Idle);
	CreateState<UDSPlayerState_Walk>(EPlayerStateType::EPS_Walk);
	CreateState<UDSPlayerState_Roll>(EPlayerStateType::EPS_Roll);
	CreateState<UDSPlayerState_SwordAttack>(EPlayerStateType::EPS_SwordAttack);
	CreateState<UDSPlayerState_Death>(EPlayerStateType::EPS_Death);

	// 초기 상태 설정
	ChangeState(EPlayerStateType::EPS_Idle);
}


// Called every frame
void UDSPlayerFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState)
	{
		CurrentState->Update(DeltaTime);
	}
}

