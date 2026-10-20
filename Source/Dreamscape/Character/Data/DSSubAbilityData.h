// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Types/DSSubAbilityTypes.h"
#include "DSSubAbilityData.generated.h"

/**
 * 
 */
UCLASS()
class DREAMSCAPE_API UDSSubAbilityData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UDSSubAbilityData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SubAbility")
	EDSSubAbilityType SubAbilityType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SubAbility", Meta = (ClampMin = "0.0"))
	float DamageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SubAbility", Meta = (ClampMin = "0.0"))
	float StaminaCost;

	// 서브 능력 사용에 필요한 충전 타임
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SubAbility", Meta = (ClampMin = "0.0"))
	float ChargeTime;

	// 투사체를 사용하는 서브 능력에 대한 속성
	// 투사체 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SubAbility|Projectile", Meta = (ClampMin = "0.0"))
	float ProjectileSpeed;
	
protected:

};
