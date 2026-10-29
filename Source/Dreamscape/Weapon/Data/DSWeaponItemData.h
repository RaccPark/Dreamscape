// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/DSItemData.h"
#include "../DSSwordWeaponBase.h"
#include "DSWeaponItemData.generated.h"

/**
 * 기본 공격 무기 한 종의 데이터.
 * 기획서 "공격 시스템"의 무기 공통 스탯 5개(콤보 수 · 공격 속도 · 공격력 배수 · 회복 시간 · 사거리)를 전부 담는다.
 * 콤보 수는 ComboActionData->MaxComboCount가 맡는다.
 *
 * 실제 피해·배속·사거리는 UDSPlayerCombatComponent가 여기 값에 플레이어 스탯을 곱해 계산한다.
 */
UCLASS()
class DREAMSCAPE_API UDSWeaponItemData : public UDSItemData
{
	GENERATED_BODY()

public:
	UDSWeaponItemData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Data")
	TSubclassOf<class ADSSwordWeaponBase> SwordWeaponClass;

	// 공격력 배수. 플레이어 최종 피해 = 플레이어 공격력 × 이 값 (기획: 기본 검 1.0 · 단거리 0.6 · 중거리 1.2 · 대검 2.0)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Data", Meta = (ClampMin = "0.0"))
	float DamageMultiplier;

	// 스탯 없이 무기 단독으로 내는 절대 피해.
	// 적 캐릭터(보스)가 이 무기를 들고 ADSSwordWeaponBase의 소켓 트레이스로 때릴 때만 쓴다.
	// 플레이어 경로는 이 값을 읽지 않는다. Phase 7(적 전투)에서 적 전용 데이터로 분리 예정.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Data|Enemy", Meta = (ClampMin = "0.0"))
	float DamageAmount;

	// 공격 속도 = 애니메이션 배속. 플레이어의 공격 속도 배수가 여기에 다시 곱해진다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Data", Meta = (ClampMin = "0.01"))
	float AttackRate;

	// 공격 후 다음 행동까지의 회복 시간 (기획: 0.5 · 0.25 · 0.4 · 0.75초)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Data", Meta = (ClampMin = "0.0"))
	float RecoveryTime;

	// 사거리. **미구현** — 판정이 무기 소켓 트레이스라 사거리를 어떻게 반영할지 추후 결정한다.
	// 기획서 항목이라 필드만 남겨 둔다. 현재는 아무 코드도 읽지 않는다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Data", Meta = (ClampMin = "0.0"))
	float AttackRange;

	// 공격 시작 시 전방으로 미는 힘. 0이면 런치 없음.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Data", Meta = (ClampMin = "0.0"))
	float LungeImpulse;

	// 이 무기를 휘두르는 플레이어 몽타주. 기획서 "각 Weapon에 있는 휘두르는 애니메이션"
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Data")
	TObjectPtr<class UAnimMontage> AttackMontage;

	// 콤보 수 · 섹션 이름 · 선입력 유효 프레임. MaxComboCount가 기획서의 "휘두르는 횟수"
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Data")
	TObjectPtr<class UDSCharacterComboActionData> ComboActionData;

	virtual void UseItem(class ACharacter* Character) override;
};
