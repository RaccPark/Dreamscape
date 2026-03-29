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
	EPS_SwordAttack,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREAMSCAPE_API UDSPlayerFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDSPlayerFSMComponent();

	UDSPlayerStateBase* GetState(EPlayerStateType StateType) const;
	void ChangeState(EPlayerStateType NewType);

	void HandleMoveInput(const FVector2D& Input);
	void HandleRollInput();
	void HandleSwordAttackInput();
	void HandleComboActionEnd();

	EPlayerStateType GetCurrentStateType() const;

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
	UDSPlayerStateBase* IdleState;
	UPROPERTY()
	UDSPlayerStateBase* WalkState;
	UPROPERTY()
	UDSPlayerStateBase* RollState;
	UPROPERTY()
	UDSPlayerStateBase* SwordAttack;

	UPROPERTY()
	TObjectPtr<class ADSCharacterPlayer> OwnerCharacter;

	EPlayerStateType CurrentStateType;

	template<typename T>
	T* CreateState(EPlayerStateType Type)
	{
		T* NewState = NewObject<T>(this);
		NewState->Initialize(this);
		PlayerStates.Add(Type, NewState);
		return NewState;
	}
};
