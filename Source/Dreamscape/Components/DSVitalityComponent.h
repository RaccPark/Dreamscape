// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DSVitalityComponent.generated.h"

// 체력이 바뀔 때마다. UI 갱신용
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVitalityHealthChanged, float, NewHealth, float, MaxHealth);
// 실제로 피해를 받았을 때(무적으로 무시된 경우는 발생하지 않는다)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVitalityDamaged, float, DamageAmount, AActor*, DamageInstigator);
// 경직 상태로 들어가야 할 때. 소유 캐릭터가 받아 상태 머신에 넘긴다.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVitalityStaggered);
// 체력이 0이 되었을 때
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVitalityDeath);

/**
 * 플레이어와 적이 공유하는 체력·피격 컴포넌트.
 *
 * 수치와 이벤트만 담당한다. 상태 전이는 하지 않는다 —
 * 소유 캐릭터가 이 컴포넌트의 이벤트를 받아 자기 상태 머신에 넘기는 방식으로 유지한다.
 *
 * 최대 체력은 UDSCharacterStatsComponent가 소유하며 여기서는 캐시만 한다.
 *
 * 무적은 이 컴포넌트가 스스로 켜지 않는다. 상태 클래스가 SetInvincible()로 켜고 끈다 —
 * 그래서 "플레이어는 구르기·경직 중 무적, 적은 경직 중에도 피해를 받는다"는
 * 기획서의 상반된 규칙이 컴포넌트를 가르지 않고도 갈린다.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DREAMSCAPE_API UDSVitalityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDSVitalityComponent();

// ==================================================
// 초기화
// ==================================================
public:
	// 스탯 컴포넌트를 물리고 체력을 최대치로 채운다.
	virtual void Initialize(class UDSCharacterStatsComponent* InStatsComponent);

	// 스탯이 바뀌었을 때 최대치를 다시 읽는다. 현재값은 새 최대치로 클램프된다.
	UFUNCTION()
	virtual void RefreshMaxValues();

// ==================================================
// 체력
// ==================================================
public:
	/**
	 * 피해를 적용한다.
	 * @return 실제로 피해를 받았으면 true. 무적·사망 상태이거나 피해량이 0이면 false.
	 *         넉백은 호출자가 이 반환값을 보고 적용해야 한다.
	 */
	UFUNCTION(BlueprintCallable, Category = "Vitality")
	bool ApplyDamage(float DamageAmount, AActor* DamageInstigator = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Vitality")
	void Heal(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Vitality")
	void SetInvincible(bool bNewInvincible);

	UFUNCTION(BlueprintPure, Category = "Vitality")
	bool IsInvincible() const { return bIsInvincible; }

	UFUNCTION(BlueprintPure, Category = "Vitality")
	bool IsDead() const { return bIsDead; }

	UFUNCTION(BlueprintPure, Category = "Vitality")
	float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category = "Vitality")
	float GetMaxHealth() const { return CachedMaxHealth; }

protected:
	/**
	 * 들어온 피해량을 이 캐릭터의 단위로 맞춘다.
	 * 기본 구현은 그대로 반환한다 — 적 캐릭터는 실수 피해를 그대로 받는다.
	 * 플레이어는 UDSPlayerVitalityComponent가 정수로 반올림하도록 오버라이드한다.
	 *
	 * 정수/실수 규칙이 갈리는 유일한 지점이다.
	 */
	virtual float SanitizeDamage(float RawDamage) const;

	class UDSCharacterStatsComponent* GetStatsComponent() const { return StatsComponent; }

// ==================================================
// 이벤트
// ==================================================
public:
	UPROPERTY(BlueprintAssignable, Category = "Vitality")
	FOnVitalityHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Vitality")
	FOnVitalityDamaged OnDamaged;

	UPROPERTY(BlueprintAssignable, Category = "Vitality")
	FOnVitalityStaggered OnStaggered;

	UPROPERTY(BlueprintAssignable, Category = "Vitality")
	FOnVitalityDeath OnDeath;

protected:
	UPROPERTY()
	TObjectPtr<class UDSCharacterStatsComponent> StatsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vitality")
	float CurrentHealth;

	// 스탯 컴포넌트에서 읽어온 최대치 캐시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vitality")
	float CachedMaxHealth;

	// 상태 클래스가 켜고 끈다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vitality")
	bool bIsInvincible;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vitality")
	bool bIsDead;
};
