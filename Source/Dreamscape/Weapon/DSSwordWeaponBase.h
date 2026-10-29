// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DSSwordWeaponBase.generated.h"

/**
 * 손에 들리는 무기 액터.
 *
 * 판정은 무기 메시의 Start~End 소켓 사이를 캡슐 트레이스해서 한다.
 * 애님 노티파이가 소유자에게 Start/Perform/End를 주고, 소유자가 여기로 넘긴다.
 * 한 번 휘두르는 동안(Start~End) 같은 액터는 한 번만 맞는다.
 *
 * 피해 적용은 소유자에 따라 다르다:
 *  - 플레이어: CollectHits()로 닿은 액터만 받아 UDSPlayerCombatComponent가 Stats × 무기 배수로 피해를 준다
 *  - 적(보스): PerformTrace()가 CollectHits 뒤 DamageAmount(절대값)를 직접 준다. Phase 7에서 정리 예정
 */
UCLASS()
class DREAMSCAPE_API ADSSwordWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	ADSSwordWeaponBase();

	// 장착 시 무기 데이터를 받는다. 적 경로가 쓸 절대 피해를 여기서 복사한다.
	virtual void InitializeWeapon(const class UDSWeaponItemData* WeaponData);

// ==================================================
// 판정 — 소켓 트레이스
// ==================================================
public:
	// 휘두르기 시작. 이번 스윙의 히트 목록을 비운다.
	void StartAttackTrace();

	// 휘두르기 끝
	void EndAttackTrace();

	// 이번 틱에 새로 닿은 액터를 돌려준다. 이미 맞은 액터와 소유자는 제외. 피해는 적용하지 않는다.
	void CollectHits(TArray<AActor*>& OutNewHits);

	// 적(보스) 전용: CollectHits 뒤 DamageAmount를 직접 적용한다.
	void PerformTrace();

	// 캡슐 트레이스 디버그 드로우 (DSWeaponTrace 콘솔 명령이 켠다)
	void SetDrawTrace(bool bEnabled) { bDrawTrace = bEnabled; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> SwordWeaponMesh;

	// 캡슐 트레이스 크기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trace", Meta = (ClampMin = "1.0"))
	float TraceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trace", Meta = (ClampMin = "1.0"))
	float TraceHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	bool bDrawTrace;

	// 적 경로가 적용하는 절대 피해. UDSWeaponItemData::DamageAmount에서 온다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DamageAmount;

	// 한 번 휘두르는 동안 이미 맞은 액터
	UPROPERTY()
	TArray<AActor*> HitActors;
};
