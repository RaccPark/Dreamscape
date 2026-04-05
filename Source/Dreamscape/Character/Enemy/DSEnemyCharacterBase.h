// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Types/DSEnemyState.h"
#include "DSEnemyCharacterBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndedDelegate)

UCLASS(Abstract)
class DREAMSCAPE_API ADSEnemyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADSEnemyCharacterBase();

	void TakeDamage(float DamageAmount);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	float CurrentHealth;

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

	virtual void OnDeath();
	virtual void DoRagdoll();

	FTimerHandle HitTimerHandle;

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
};
