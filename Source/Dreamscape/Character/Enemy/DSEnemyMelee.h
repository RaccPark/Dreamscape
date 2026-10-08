// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/DSEnemyCharacterBase.h"
#include "DSEnemyMelee.generated.h"

/**
 *
 */
UCLASS()
class DREAMSCAPE_API ADSEnemyMelee : public ADSEnemyCharacterBase
{
	GENERATED_BODY()

public:
	ADSEnemyMelee();

protected:
	virtual void BeginPlay() override;

	virtual void OnDeath() override;

	// 일반 적은 피격 시 히트 리액션 몽타주 + 블랙보드 경직 플래그를 탄다.
	virtual void OnDamaged(float DamageAmount, EDSDamageType DamageType) override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;
	void UpdateWalkSpeed(float NewSpeed);

public:
	void StartMeleeTrace();
	void PerformMeleeTrace();
	void EndMeleeTrace();

protected:
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName MeleeSocket;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MeleeTraceRadius;

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bShowDebugTrace;

private:
	// 이미 공격이 적용된 액터들을 추적하여 중복 공격 방지
	UPROPERTY()
	TArray<AActor*> HitActors;

	// 판정 보정(애니메이션이 빠를 경우 무시되는 문제를 방지)을 위한 이전 프레임의 소켓 위치
	FVector PreviousSocketLocation;
};
