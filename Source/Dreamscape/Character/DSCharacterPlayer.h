// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DSCharacterBase.h"
#include "Interface/DSDamageableInterface.h"
#include "Interface/DSAttackTraceInterface.h"
#include "DSCharacterPlayer.generated.h"

/**
 *
 */

UCLASS()
class DREAMSCAPE_API ADSCharacterPlayer : public ADSCharacterBase, public IDSDamageableInterface, public IDSAttackTraceInterface
{
	GENERATED_BODY()
	
public:
	ADSCharacterPlayer();

protected:
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Character Control Section
protected:
	// Character Movement Component
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCharacterMovementComponent> CharacterMovementComponent;

	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class UDSCharacterControlData* CharacterControlData) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CameraPeekAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MousePosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SwordAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	float TurnInterpSpeed;
	
	void HandleMove(const struct FInputActionValue& Value);	// 기본 움직임 Input을 받으면 처음에는 여기로

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

	void StartRoll(const struct FInputActionValue& Value);

	void StartFall();
	void StopFall();

	void OnPeekStarted(const struct FInputActionValue& Value);
	void OnPeekEnded(const struct FInputActionValue& Value);
	void OnMouseInput(const struct FInputActionValue& Value);

	void SwordAttack(const struct FInputActionValue& Value);

	void OnPausePressed(const struct FInputActionValue& Value);

	void OnInteractPressed(const struct FInputActionValue& Value);

	// 데이터 에셋이 지정한 기준 이동 속도. 여기에 스탯 배수를 곱해 최종 속도를 만든다.
	float DefaultMaxWalkSpeed;
	float MaxWalkSpeed;

	ECharacterControlType CurrentCharacterControlType;

	// 기준 이동 속도에 UDSPlayerStatsComponent의 이동 속도 배수를 적용한다.
	// SetCharacterControlData()가 MaxWalkSpeed를 덮어쓰므로 반드시 그 뒤에 호출되어야 한다.
	UFUNCTION()
	void ApplyMoveSpeedFromStats();

protected:
	TObjectPtr<class USkeletalMesh> DefaultSkeletonMesh;

	TSubclassOf<class UAnimInstance> DefaultAnimClass;

protected:
	// Camera Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// Camera Peek Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDSCameraPeekComponent> CameraPeekComponent;

	// 카메라 Yaw 회전 기믹용 보간 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	float CameraYawInterpSpeed;

	// 현재 적용 중인 카메라 Yaw (보간 결과)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	float CurrentCameraYaw;

	// 도달하려는 카메라 Yaw
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	float TargetCameraYaw;

	void UpdateCameraYaw(float DeltaTime);

	// Player FSM Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDSPlayerFSMComponent> PlayerFSMComponent;

	// 스탯 컴포넌트. 최대 체력·스테미나와 업그레이드 포인트의 소유자다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDSPlayerStatsComponent> StatsComponent;

	// 현재 체력·스테미나와 피격 상태. 적과 공유하는 UDSVitalityComponent의 플레이어 파생
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDSPlayerVitalityComponent> VitalityComponent;

	// 기본 공격 — 콤보 · 히트박스 판정 · 피해 계산 · 무기 장착
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDSPlayerCombatComponent> CombatComponent;

	// Inventory Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDSInventoryComponent> InventoryComponent;

	// Interaction Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDSInteractionComponent> InteractionComponent;

public:
	UCameraComponent* GetCamera() const;

	class USpringArmComponent* GetCameraBoom() const;

	/**
	 * 화면 기준 "앞"의 월드 Yaw.
	 * 이동 입력과 카메라 연출이 같은 값을 보도록 하는 단일 기준점이다.
	 * 스프링암의 bUsePawnControlRotation / bInherit* 설정을 엔진 API가 반영해 주므로
	 * 데이터 에셋에서 플래그를 바꿔도 이동 기준이 따라온다.
	 */
	UFUNCTION(BlueprintPure, Category = "Camera")
	float GetCameraYaw() const;

	// 카메라 회전 기믹용. bInstant면 보간 없이 즉시 적용
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetCameraYaw(float NewYaw, bool bInstant = false);

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void AddCameraYaw(float DeltaYaw, bool bInstant = false);

	UDSPlayerFSMComponent* GetPlayerFSMComponent() const;

	UFUNCTION(BlueprintPure, Category = "Player")
	class UDSPlayerStatsComponent* GetStatsComponent() const;

	UFUNCTION(BlueprintPure, Category = "Player")
	class UDSPlayerVitalityComponent* GetVitalityComponent() const;

	UFUNCTION(BlueprintPure, Category = "Player")
	class UDSPlayerCombatComponent* GetCombatComponent() const;

	class UInputMappingContext* GetDefaultMappingContext() const;

	class UDSInventoryComponent* GetInventoryComponent() const;

	class UDSInteractionComponent* GetInteractionComponent() const;

// Animation Montage Section
protected:
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<class UAnimMontage> RollMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<class UAnimMontage> HitMontage;

// Animation Montage Play Section
public:
	void PlayRollMontage();
	void PlayHitMontage();


public:
	void Move(const struct FInputActionValue& Value);

	// 공격 시작 시 마우스 커서 방향으로 즉시 회전. SwordAttack 상태가 부른다.
	void RotateCharacterToMouseCursor();

	virtual void OnDeath() override;
	void OnRollEnd();

// ==================================================
// Attack Trace Interface Section
// ==================================================
// UDSAnimNotifyState_Attack이 소유 액터를 이 인터페이스로 캐스팅하므로 캐릭터가 받는다.
// 실제 판정은 CombatComponent에 위임한다.
public:
	virtual void StartAttackTrace() override;
	virtual void PerformAttackTrace() override;
	virtual void EndAttackTrace() override;

protected:
	// CombatComponent의 공격 시퀀스 종료 이벤트를 FSM으로 넘긴다.
	UFUNCTION()
	void HandleAttackSequenceEnded();


// ==================================================
// Damageable Interface Section
// ==================================================
public:
	// Interface
	virtual void ApplyDamage(float DamageAmount, EDSDamageType DamageType) override;
	virtual void ApplyDamageWithKnockback(float DamageAmount, EDSDamageType DamageType, const FVector& KnockbackDirection, float KnockbackStrength) override;

protected:
	// VitalityComponent의 이벤트를 FSM으로 넘기는 중계 지점.
	// 컴포넌트가 FSM을 직접 부르지 않도록 캐릭터가 중재한다.
	UFUNCTION()
	void HandleStaggered();

	UFUNCTION()
	void HandleVitalityDeath();

// ==================================================
// Inventory Test Section
// ==================================================
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDSWeaponItemData> DefaultWeaponItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDSWeaponItemData> TestWeaponItemData01;


// ==================================================
// Tutorial Section
// ==================================================
public:
	// 카메라 피킹(Shift) 사용 시 브로드캐스트
	FSimpleMulticastDelegate OnCameraPeekStartedDelegate;

// ==================================================
// Camera Tuning Section (PIE 콘솔 전용)
// ==================================================
public:
	// DSCam <FOV> <ArmLength> : 0 이하 인자는 현재 값 유지. 적용 후 화면 가로 폭을 로그로 출력
	UFUNCTION(Exec)
	void DSCam(float NewFOV = 0.0f, float NewArmLength = 0.0f);

	// DSCamYaw <Yaw> : 카메라를 해당 월드 Yaw로 회전 (절대값)
	UFUNCTION(Exec)
	void DSCamYaw(float NewYaw);

	// DSCamRotate <DeltaYaw> : 현재 각도에서 상대 회전. 인자를 생략하면 90도씩 돈다.
	// 카메라가 회전해도 이동이 계속 화면 기준 앞으로 가는지 검증하는 용도.
	UFUNCTION(Exec)
	void DSCamRotate(float DeltaYaw = 90.0f);

// ==================================================
// Stat / Vitality Tuning Section (PIE 콘솔 전용)
// ==================================================
public:
	// DSVital : 현재 체력·스테미나·무적 여부와 파생 스탯을 로그로 출력
	UFUNCTION(Exec)
	void DSVital();

	// DSDamage <Amount> : 자기 자신에게 피해를 준다. 무적 판정과 경직 전이를 그대로 탄다.
	UFUNCTION(Exec)
	void DSDamage(float Amount = 1.0f);

	// DSStamina <Amount> : 스테미나를 소모한다. 자동 회복 확인용
	UFUNCTION(Exec)
	void DSStamina(float Amount = 1.0f);

	// DSUpgrade <Attack|SubAttack|Speed|Mobility> <포인트> : 업그레이드 포인트를 올린다
	UFUNCTION(Exec)
	void DSUpgrade(const FString& StatName, int32 Amount = 1);

// ==================================================
// Combat Tuning Section (PIE 콘솔 전용)
// ==================================================
public:
	// DSCombat : 현재 무기 · 최종 피해 · 배속 · 회복 시간 · 콤보 창을 로그로 출력
	UFUNCTION(Exec)
	void DSCombat();

	// DSWeaponTrace <0|1> : 무기 캡슐 트레이스 디버그 드로우 토글
	UFUNCTION(Exec)
	void DSWeaponTrace(int32 bEnabled = 1);
};
