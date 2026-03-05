// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DSPlayerStateBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class DREAMSCAPE_API UDSPlayerStateBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(class UDSPlayerFSMComponent* PlayerFSMComponent);

	virtual void Enter();
	virtual void Update(float DeltaTime);
	virtual void Exit();

	virtual void OnMove(const FVector2D& Value);
	virtual void OnRoll();

protected:
	UPROPERTY()
	TObjectPtr<class UDSPlayerFSMComponent> OwnerFSMComponent;

	UPROPERTY()
	TObjectPtr<class ADSCharacterPlayer> PlayerCharacter;

};

