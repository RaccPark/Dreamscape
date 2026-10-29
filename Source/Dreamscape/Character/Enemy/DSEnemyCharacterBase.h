// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Types/DSEnemyState.h"
#include "Interface/DSDamageableInterface.h"
#include "DSEnemyCharacterBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndedDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(Abstract)
class DREAMSCAPE_API ADSEnemyCharacterBase : public ACharacter, public IDSDamageableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADSEnemyCharacterBase();

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 스탯 컴포넌트. 최대 체력의 소유자다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	TObjectPtr<class UDSCharacterStatsComponent> StatsComponent;

	// 현재 체력과 피격 상태. 플레이어와 공유하는 컴포넌트다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	TObjectPtr<class UDSVitalityComponent> VitalityComponent;

	UPROPERTY(EditAnywhere, Category = "Chase")
	float ChaseStartRange;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackStartRange;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackDamage;

	UPROPERTY(EditAnywhere, Category = "Movement")
	TObjectPtr<class UCharacterMovementComponent> CharacterMovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Animation")
	TObjectPtr<class UDSEnemyAnimInstance> EnemyAnimInstance;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<class UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<class UAnimMontage> DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EDSEnemyState CurrentState;

	virtual void OnDeath();
	virtual void DoRagdoll();

	// VitalityComponent의 이벤트를 받는 중계 지점.
	// 컴포넌트가 상태 머신·블랙보드를 직접 건드리지 않도록 캐릭터가 중재한다.
	UFUNCTION()
	void HandleDamaged(const FDSDamageInfo& DamageInfo);

	UFUNCTION()
	void HandleVitalityDeath();

	// 히트 리액션. 파생이 정한다
	// 일반 적은 경직, 몽타주, 보스는 기획서상 경직하지 않으므로 비워 둔다.
	// 피해 종류를 통하여 특정 공격(폭발)의 경우 경직, 그 외의 보조 공격은 경직 없음 규칙을 만들 수 있다.
	virtual void OnDamaged(const FDSDamageInfo& DamageInfo);

	FTimerHandle HitTimerHandle;

	void OnStateChanged(EDSEnemyState NewState);

public:
	// IDSDamageableInterface — 파생이 아니라 여기서 한 번만 구현한다.
	virtual void ApplyDamage(const FDSDamageInfo& DamageInfo) override;

	class UDSCharacterStatsComponent* GetStatsComponent() const { return StatsComponent; }
	class UDSVitalityComponent* GetVitalityComponent() const { return VitalityComponent; }

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "AI")
	virtual void Attack();

	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	FOnAttackEndedDelegate OnAttackEnded;

	float GetChaseStartRange() const;
	float GetAttackStartRange() const;

	EDSEnemyState GetState() const;
	void SetState(EDSEnemyState NewState);

	FOnDeath OnDeathDelegate;

	virtual void PlayLandingSequnce();
};
