// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DSCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quarter,
};

UCLASS()
class DREAMSCAPE_API ADSCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADSCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetCharacterControlData(const class UDSCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UDSCharacterControlData*> CharacterControlManager;

	// 체력은 여기 있지 않다. 플레이어는 UDSVitalityComponent가, 적 캐릭터는 각자 소유한다.

protected:
	virtual void OnDeath();

};
