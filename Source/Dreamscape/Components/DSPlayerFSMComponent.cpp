// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DSPlayerFSMComponent.h"
#include "Components/PlayerStateBase/DSPlayerStateBase.h"
#include "Character/DSCharacterPlayer.h"

#include "Components/PlayerStateBase/DSPlayerState_Idle.h"
#include "Components/PlayerStateBase/DSPlayerState_Walk.h"
#include "Components/PlayerStateBase/DSPlayerState_Roll.h"
#include "Components/PlayerStateBase/DSPlayerState_SwordAttack.h"

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

	// 새로운 상태 진입
	CurrentStateType = NewType;
	CurrentState = GetState(NewType);

	if (CurrentState)
	{
		CurrentState->Enter();
	}

}

void UDSPlayerFSMComponent::HandleMoveInput(const FVector2D& Input)
{
	if (CurrentState)
	{
		CurrentState->OnMove(Input);
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

EPlayerStateType UDSPlayerFSMComponent::GetCurrentStateType() const
{
	return CurrentStateType;
}


// Called when the game starts
void UDSPlayerFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ADSCharacterPlayer>(GetOwner());
	
	CreateState<UDSPlayerState_Idle>(EPlayerStateType::EPS_Idle);
	CreateState<UDSPlayerState_Walk>(EPlayerStateType::EPS_Walk);
	CreateState<UDSPlayerState_Roll>(EPlayerStateType::EPS_Roll);
	CreateState<UDSPlayerState_SwordAttack>(EPlayerStateType::EPS_SwordAttack);

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

