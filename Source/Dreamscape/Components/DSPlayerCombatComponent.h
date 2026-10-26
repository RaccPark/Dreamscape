// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/DSComboCommand.h"
#include "DSPlayerSubAbilityComponent.h"
#include "DSPlayerCombatComponent.generated.h"

// 공격 시퀀스(콤보 + 회복 시간)가 완전히 끝났을 때. 캐릭터가 받아 FSM에 넘긴다.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackSequenceEnded);

/**
 * 플레이어의 기본 공격을 담당하는 컴포넌트.
 *
 * 기획서 "최종 공격은 PlayerCombatComponent로 한다"에 따라
 * 콤보, 선입력 버퍼, 히트박스 판정, 피해 계산, 무기 장착이 전부 여기 있다.
 *
 * 피해 · 배속 · 사거리 · 회복 시간은 저장하지 않고 매번
 * (UDSPlayerStatsComponent 파생값) * (UDSWeaponItemData 값)으로 계산한다.
 *
 * 판정은 무기 액터(ADSSwordWeaponBase)의 소켓 캡슐 트레이스가 한다 - 칼이 닿는 적이 순서대로, 한 스윙에 한 번씩.
 * 시작·종료 타이밍은 UDSAnimNotifyState_Attack이 IDSAttackTraceInterface를 통해 캐릭터에 주고,
 * 캐릭터가 여기로, 여기서 무기로 넘긴다. 무기는 닿은 액터만 돌려주고 피해는 여기서 준다.
 *
 * 상태 전이는 하지 않는다. FSM은 OnAttackSequenceEnded를 구독한 캐릭터가 중재한다.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DREAMSCAPE_API UDSPlayerCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDSPlayerCombatComponent();

protected:
	virtual void BeginPlay() override;

// ==================================================
// 무기
// ==================================================
public:
	// 무기를 스폰해 등 뒤 소켓에 붙인다. 기존 무기는 파괴한다.
	void EquipWeapon(const class UDSWeaponItemData* NewWeaponData);

	// 장착된 무기를 다른 소켓으로 옮긴다 (손 ↔ 등)
	void SetWeaponSocket(FName SocketName);

	class ADSSwordWeaponBase* GetEquippedWeapon() const { return EquippedWeapon; }
	const class UDSWeaponItemData* GetEquippedWeaponData() const { return EquippedWeaponData; }

// ==================================================
// 파생값 — Stats * 무기 데이터
// ==================================================
public:
	// 최종 피해 = 플레이어 공격력 * 무기 배수
	UFUNCTION(BlueprintPure, Category = "Combat")
	float CalculateDamage() const;

	// 몽타주 배속 = 무기 공격 속도 * 플레이어 공격 속도 배수
	UFUNCTION(BlueprintPure, Category = "Combat")
	float GetAttackPlayRate() const;

	// 회복 시간 = 무기 회복 시간 / 플레이어 회복 속도 배수
	UFUNCTION(BlueprintPure, Category = "Combat")
	float GetRecoveryTime() const;

	// 보조 공격 최종 피해
	float CalculateSubAbilityDamage(const class UDSSubAbilityData* SubAbilityData) const;

	UFUNCTION(BlueprintPure, Category = "Combat")
	bool IsRecovering() const { return bIsRecovering; }

	UFUNCTION(BlueprintPure, Category = "Combat")
	int32 GetCurrentCombo() const { return CurrentCombo; }

	// 콤보 N타(1부터)의 선입력 창이 열리는 시각(초, 현재 배속 기준, 블렌드 아웃 클램프 반영). 튜닝용
	float GetComboWindowOpenTime(int32 ComboIndex) const;

protected:
	// 창 열림 시각 계산. Raw = 데이터 그대로, Latest = 블렌드 아웃 직전. 반환값은 둘 중 작은 쪽.
	// SectionOverride가 있으면 그 섹션 길이를 쓴다 (재생 중에는 현재 섹션을 넘긴다 - 이름 규약과 무관하게 정확하다).
	float ComputeComboWindowOpenTime(int32 ComboIndex, float& OutRawOpenTime, float& OutLatestOpenTime, FName SectionOverride = NAME_None) const;

	// 몽타주를 재생하고 종료 델리게이트를 묶는다. 처음 시작과 재시작이 공유한다.
	void PlayAttackMontage(class UAnimInstance* AnimInstance);

public:

// ==================================================
// 콤보 (커맨드 패턴)
// ==================================================
// 선입력 창은 EffectiveFrameCounts[N]에서 "열리고" 그 섹션이 끝날 때까지 "열려 있다".
//  - 창이 열리기 전 입력  -> 버퍼에 적재, 창이 열리는 순간 다음 타로
//  - 창이 열린 뒤 입력    -> 즉시 다음 타로
//  - 섹션이 끝날 때까지 입력 없음 -> 몽타주 종료 -> 회복
// 배속이 올라가도 창이 사람 반응보다 먼저 닫히는 일이 없다.
public:
	// 공격 입력 진입점. 첫 입력은 즉시 실행, 회복 중 입력은 무시.
	void ProcessComboCommand();

	// 콤보 1타 시작. DSSwordAttackCommand가 호출한다.
	void ComboActionBegin();

protected:
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool bInterrupted);
	void SetComboCheckTimer();
	void ComboCheck();
	void OnRecoveryFinished();

	// 다음 콤보 섹션으로 점프. 창이 열려 있고 상한 미만일 때만 부른다.
	void AdvanceCombo();
	bool CanAdvanceCombo() const;

// ==================================================
// 판정 — 캐릭터의 IDSAttackTraceInterface가 위임한다. 실제 트레이스는 무기가 한다
// ==================================================
public:
	void StartAttackTrace();
	void PerformAttackTrace();
	void EndAttackTrace();

	// 무기 캡슐 트레이스 디버그 드로우 (DSWeaponTrace 콘솔 명령)
	void SetDrawWeaponTrace(bool bEnabled);

// ===
// 보조 공격 피해
// 보조 공격의 실제 전투 적용은 CombatComponent가 한다.
// 판정(투사체, 범위)은 UDSPlayerSubAbilityComponent가 한다.
// 닿은 대상만 여기로 넘긴다.
// ===
public:
	void ApplySubAbilityDamage(AActor* Target, const class UDSSubAbilityData* SubAbilityData, const FVector& KnockbackDirection);

// ==================================================
// 이벤트
// ==================================================
public:
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnAttackSequenceEnded OnAttackSequenceEnded;

// ==================================================
// 피해 설정
// ==================================================
protected:
	// 피격 대상에게 주는 넉백 힘. 넉백 저항은 Phase 7의 EnemyStats에서 얹는다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", Meta = (ClampMin = "0.0"))
	float KnockbackStrength;

	// 장착 시 무기에 넘길 디버그 드로우 상태
	bool bDrawWeaponTrace;

// ==================================================
// 상태
// ==================================================
protected:
	UPROPERTY()
	TObjectPtr<class ADSCharacterPlayer> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<class UDSPlayerStatsComponent> StatsComponent;

	UPROPERTY()
	TObjectPtr<class ADSSwordWeaponBase> EquippedWeapon;

	UPROPERTY()
	TObjectPtr<const class UDSWeaponItemData> EquippedWeaponData;

	// 현재 콤보 단계. 0이면 공격 중이 아님
	int32 CurrentCombo;

	FTimerHandle ComboTimerHandle;
	FTimerHandle RecoveryTimerHandle;

	// 선입력 버퍼. 창이 열리기 전에 들어온 다음 콤보 입력
	TUniquePtr<IDSComboCommand> PendingComboCommand;

	// 현재 타의 선입력 창이 열렸는가. 열린 뒤에는 섹션이 끝날 때까지 유지된다.
	bool bComboWindowOpen;

	bool bIsRecovering;
};
