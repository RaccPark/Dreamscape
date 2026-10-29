// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSDamageTypes.generated.h"

class AActor;

/**
 * 기본 공격, 보조 공격, 폭발, 적 공격 등
 * 공격 종류를 분류하기 위한 enum
 * 공격 종류를 분류하여 폭발 경직 등의 효과를 적용할 수 있음.
 */
UENUM(BlueprintType)
enum class EDSDamageType : uint8
{
	MainAttack UMETA(DisplayName = "Main Attack"),
	SubAttack UMETA(DisplayName = "Sub Attack"),
	Explosion UMETA(DisplayName = "Explosion"),
	EnemyAttack UMETA(DisplayName = "Enemy Attack"),
};

/**
 * 피해 한 번에 실리는 정보. IDSDamageableInterface::ApplyDamage가 이것 하나만 받음
 * 항목이 늘어도 인터페이스·구현체 시그니처는 바뀌지 않는다 - 필드만 추가하고, 채우는 쪽과 읽는 쪽만 고친다.
 * Victim은 ApplyDamage를 받는 쪽에서의 this임
 */
USTRUCT(BlueprintType)
struct DREAMSCAPE_API FDSDamageInfo
{
	GENERATED_BODY()

	FDSDamageInfo() {}

	// 기본값 세팅 x
	// 피해는 반드시 정보가 있어야 한다
	FDSDamageInfo(float InAmount, EDSDamageType InType, AActor* InAttacker)
		: Amount(InAmount), Type(InType), Attacker(InAttacker)
	{
	}

	// 최종 피해량 (Stats * 배수가 끝난 값)
	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	float Amount = 0.0f;

	// 피해 종류
	// 받는 쪽이 경직, 히트 리액션을 가름
	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	EDSDamageType Type = EDSDamageType::MainAttack;

	// 공격한 캐릭터
	// 투사체로 맞혔어도 투사체가 아니라 쏜 캐릭터를 넣는다
	UPROPERTY()
	TWeakObjectPtr<AActor> Attacker;

	// 넉백 방향
	// 항상 수평, 길이 1
	// SetKnockback / SetKnockbackFrom으로만 채운다
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FVector KnockbackDirection = FVector::ZeroVector;

	// 넉백 세기
	// 0이면 넉백 없음
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	float KnockbackStrength = 0.0f;

	// 방향을 이미 알 때 (화살의 진행 방향 등)
	void SetKnockback(const FVector& Direction, float Strength);

	// 중심(무기, 폭발 지점)에서 대상 쪽으로
	void SetKnockbackFrom(const FVector& FromLocation, const AActor* Target, float Strength);

	bool HasKnockback() const { return KnockbackStrength > 0.0f && !KnockbackDirection.IsNearlyZero(); }
};
