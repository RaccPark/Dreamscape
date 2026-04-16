// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DSPlayerFSMComponent.generated.h"

class UDSPlayerStateBase;

UENUM(BlueprintType)
enum class EPlayerStateType : uint8
{
	EPS_Idle,
	EPS_Walk,
	EPS_Roll,
	EPS_Fall,
	EPS_Land,
	EPS_Hit,
	EPS_SwordAttack,
	EPS_Death,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChanged, EPlayerStateType, NewState);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREAMSCAPE_API UDSPlayerFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDSPlayerFSMComponent();

	UDSPlayerStateBase* GetState(EPlayerStateType StateType) const;
	void ChangeState(EPlayerStateType NewType);

	void HandleMoveInput(const struct FInputActionValue& Value);
	void HandleRollInput();
	void HandleRollEnd();
	void HandleSwordAttackInput();
	void HandleComboActionEnd();
	void HandleFall();
	void HandleLand();
	void HandleHit();
	void HandleDeath();

	// 현재 상태를 유지한채 새로운 상태 스택 쌓기
	void PushPlayerState(EPlayerStateType NewType);
	// 현재 상태 종료 후 이전 상태로 복귀
	void PopPlayerState();

	EPlayerStateType GetCurrentStateType() const;
	EPlayerStateType GetPreviousStateType() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	UDSPlayerStateBase* CurrentState;

	TMap<EPlayerStateType, UDSPlayerStateBase*> PlayerStates;

	UPROPERTY()
	TObjectPtr<class ADSCharacterPlayer> OwnerCharacter;

	// Pushdown Automata
	UPROPERTY(VisibleAnywhere)
	TArray<EPlayerStateType> StateStack;

	template<typename T>
	T* CreateState(EPlayerStateType Type)
	{
		T* NewState = NewObject<T>(this);
		NewState->Initialize(this);
		PlayerStates.Add(Type, NewState);
		return NewState;
	}

	void UpdateCurrentStatePointer();

public:
	FOnStateChanged OnStateChangedDelegate;
};
