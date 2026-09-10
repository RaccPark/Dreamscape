// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DSCharacterStatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterStatsChanged);

/**
 * 플레이어와 적이 공유하는 스탯 컴포넌트의 베이스.
 *
 * 여기서 보장하는 것은 하나다 — **최대 체력의 단일 소유자**.
 * UDSVitalityComponent는 이 타입만 알고 최대치를 조회한다.
 *
 * 적 캐릭터는 이 클래스를 그대로 쓰고, 플레이어는 UDSPlayerStatsComponent가 상속해
 * 업그레이드 포인트와 파생 스탯을 얹는다.
 * (기획서의 EnemyStatsComponent / BossStatsComponent 상속 구조도 여기서 갈라진다)
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DREAMSCAPE_API UDSCharacterStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDSCharacterStatsComponent();

	// 소유 캐릭터의 PostInitializeComponents에서 호출된다. 파생이 오버라이드한다.
	virtual void InitializeStats();

	// 최대 체력. 파생이 다른 출처를 쓰면 오버라이드한다.
	UFUNCTION(BlueprintPure, Category = "Stats")
	virtual int32 GetMaxHealth() const;

public:
	// 스탯이 바뀌면 브로드캐스트. Vitality의 최대치 갱신·이동 속도 재적용·UI가 여기에 물린다.
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnCharacterStatsChanged OnStatsChanged;

protected:
	// 적 캐릭터가 쓰는 최대 체력. 플레이어는 UDSPlayerStatsComponent가 스냅샷 값으로 대체한다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", Meta = (ClampMin = "1"))
	int32 BaseMaxHealth;
};
