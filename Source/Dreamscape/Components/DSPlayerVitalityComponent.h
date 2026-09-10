// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DSVitalityComponent.h"
#include "DSPlayerVitalityComponent.generated.h"

// 스테미나가 바뀔 때마다
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVitalityStaminaChanged, float, NewStamina, float, MaxStamina);

/**
 * 플레이어 전용 체력 컴포넌트.
 *
 * 공통 UDSVitalityComponent에 두 가지를 더한다:
 *  - 스테미나 (보조 공격 소모 자원). 적 캐릭터에는 없는 개념이라 여기로 내려왔다
 *  - 정수 단위 피해. 플레이어 체력은 HP 셀 단위라 소수 피해를 반올림한다
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DREAMSCAPE_API UDSPlayerVitalityComponent : public UDSVitalityComponent
{
	GENERATED_BODY()

public:
	UDSPlayerVitalityComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Initialize(class UDSCharacterStatsComponent* InStatsComponent) override;

	virtual void RefreshMaxValues() override;

// ==================================================
// 스테미나
// ==================================================
public:
	UFUNCTION(BlueprintPure, Category = "Vitality")
	bool CanConsumeStamina(float Amount) const;

	// 충분하면 소모하고 true, 모자라면 아무것도 하지 않고 false
	UFUNCTION(BlueprintCallable, Category = "Vitality")
	bool ConsumeStamina(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Vitality")
	void RestoreStamina(float Amount);

	UFUNCTION(BlueprintPure, Category = "Vitality")
	float GetCurrentStamina() const { return CurrentStamina; }

	UFUNCTION(BlueprintPure, Category = "Vitality")
	float GetMaxStamina() const { return CachedMaxStamina; }

	UPROPERTY(BlueprintAssignable, Category = "Vitality")
	FOnVitalityStaminaChanged OnStaminaChanged;

protected:
	// 플레이어 체력은 정수 칸이다. 무기 배수가 만든 소수 피해를 정수로 맞춘다.
	// 반올림해서 0이 되더라도 공격이 무효가 되지는 않도록 최소 1은 깎는다.
	virtual float SanitizeDamage(float RawDamage) const override;

	// 스테미나 최대치 조회용. Initialize에서 캐스팅해 캐시한다.
	UPROPERTY()
	TObjectPtr<class UDSPlayerStatsComponent> PlayerStatsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vitality")
	float CurrentStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vitality")
	float CachedMaxStamina;
};
