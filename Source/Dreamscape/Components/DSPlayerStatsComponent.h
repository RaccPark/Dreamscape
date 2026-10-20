// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DSCharacterStatsComponent.h"
#include "Types/DSPlayerStatTypes.h"
#include "DSPlayerStatsComponent.generated.h"

/**
 * 플레이어의 스탯을 소유하는 컴포넌트.
 *
 * 최대 체력·최대 스테미나의 단일 소유자다. UDSVitalityComponent는 여기서 조회해 캐시할 뿐이다.
 * 기준값은 UDSPlayerDataAsset에서 오고, 업그레이드 포인트는 FDSPlayerStatsSnapshot에 쌓인다.
 * 파생 스탯(공격력·배속·이동속도 배수 등)은 저장하지 않고 매번 계산한다.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DREAMSCAPE_API UDSPlayerStatsComponent : public UDSCharacterStatsComponent
{
	GENERATED_BODY()

public:
	UDSPlayerStatsComponent();

// ==================================================
// 초기화 / 스냅샷
// ==================================================
public:
	// 데이터 에셋 값으로 스냅샷을 초기화한다. PostInitializeComponents에서 호출된다.
	virtual void InitializeStats() override;

	// 레벨 전환·세이브에서 복원할 때 사용
	void ApplySnapshot(const FDSPlayerStatsSnapshot& NewSnapshot);

	const FDSPlayerStatsSnapshot& GetSnapshot() const { return Snapshot; }

	const class UDSPlayerDataAsset* GetPlayerData() const { return PlayerData; }

// ==================================================
// 파생 스탯 조회
// ==================================================
public:
	virtual int32 GetMaxHealth() const override;

	UFUNCTION(BlueprintPure, Category = "Player Stats")
	int32 GetMaxStamina() const;

	UFUNCTION(BlueprintPure, Category = "Player Stats")
	float GetStaminaRegenPerSecond() const;

	// 기본 공격력 + (포인트 * 포인트당 증가량)
	UFUNCTION(BlueprintPure, Category = "Player Stats")
	float GetAttackPower() const;

	UFUNCTION(BlueprintPure, Category = "Player Stats")
	float GetSubAttackPower() const;

	// 애니메이션 배속에 곱할 배수. 1.0 + 0.05 * 포인트
	UFUNCTION(BlueprintPure, Category = "Player Stats")
	float GetAttackSpeedMultiplier() const;

	// 이동 속도에 곱할 배수. 1.0 + 0.05 * 포인트
	UFUNCTION(BlueprintPure, Category = "Player Stats")
	float GetMoveSpeedMultiplier() const;

	// 공격 사거리(히트박스 길이)에 곱할 배수. 1.0 + 0.02 * 포인트
	UFUNCTION(BlueprintPure, Category = "Player Stats")
	float GetAttackRangeMultiplier() const;

	// 구르기 쿨타임. 하한 아래로는 내려가지 않는다.
	UFUNCTION(BlueprintPure, Category = "Player Stats")
	float GetRollCooldown() const;

	// 무기의 회복 시간에 플레이어의 회복 속도 배수를 적용한 최종값
	UFUNCTION(BlueprintPure, Category = "Player Stats")
	float GetAttackRecoveryTime(float WeaponRecoveryTime) const;

	// 서브 공격 충전 속도 배수
	// 실제 충전 시간 = ChargeTime / 본 함수 리턴 값
	UFUNCTION(BlueprintPure, Category = "Player Stats")
	float GetSubAttackChargeRateMultiplier() const;

	// 보조 공격 발사 후 다음 행동까지의 회복 시간
	UFUNCTION(BlueprintPure, Category = "Player Stats")
	float GetSubAttackRecoveryTime() const;

	UFUNCTION(BlueprintPure, Category = "Player Stats")
	int32 GetUpgradePoint(EDSPlayerStatType StatType) const;

// ==================================================
// 변경
// ==================================================
public:
	// 포인트를 올린다. 상한에 걸리면 false.
	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	bool AddUpgradePoint(EDSPlayerStatType StatType, int32 Amount = 1);

	// 최대 체력 해금. 기획서에 업그레이드 축이 없으므로 별도 획득(수집품·퍼즐 보상)용 API로 열어둔다.
	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	void AddMaxHealth(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	void AddMaxStamina(int32 Amount);

protected:
	// 비어 있으면 InitializeStats()가 CDO 기본값으로 임시 인스턴스를 만들어 채운다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	TObjectPtr<class UDSPlayerDataAsset> PlayerData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
	FDSPlayerStatsSnapshot Snapshot;
};
