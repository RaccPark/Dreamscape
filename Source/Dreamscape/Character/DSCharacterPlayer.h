// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DSCharacterBase.h"
#include "Interface/DSDamageableInterface.h"
#include "Interface/DSAttackTraceInterface.h"
#include "Character/DSComboCommand.h"
#include "DSCharacterPlayer.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, int32, NewHelath);

UCLASS()
class DREAMSCAPE_API ADSCharacterPlayer : public ADSCharacterBase, public IDSDamageableInterface, public IDSAttackTraceInterface
{
	GENERATED_BODY()
	
public:
	ADSCharacterPlayer();

protected:
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

	float DefaultMaxWalkSpeed;
	float MaxWalkSpeed;

	bool bIsInvincible;

	ECharacterControlType CurrentCharacterControlType;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDSPlayerFSMComponent> PlayerFSMComponent;

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


// Combo Action Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UAnimMontage> SwordAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDSCharacterComboActionData> ComboActionData;

	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	void SetComboCheckTimer();
	void ComboCheck();

	int CurrentCombo;

	FTimerHandle ComboTimerHandle;
	TUniquePtr<IDSComboCommand> PendingComboCommand;
public:
	void ComboActionBegin();

	void Move(const struct FInputActionValue& Value);

	void RotateCharacterToMouseCursor();
	void ProcessComboCommand();

	virtual void OnDeath() override;
	void OnRollEnd();
	
// ==================================================
// Weapon Section
// ==================================================
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ADSSwordWeaponBase> SwordWeaponToEquip;

	class ADSSwordWeaponBase* EquippedSwordWeapon;

public:
	// 무기 장착
	void EquipSwordWeapon(const class UDSWeaponItemData* NewWeaponData);

public:
	// 소켓 위치에 따른 무기 장착
	void SetEquippedWeaponSocket(FName SocketName);

	class ADSSwordWeaponBase* GetEquippedSwordWeapon() const;

// ==================================================
// Attack Trace Interface Section(For Weapon)
// ==================================================
public:
	virtual void StartAttackTrace() override;
	virtual void PerformAttackTrace() override;
	virtual void EndAttackTrace() override;


// ==================================================
// Damageable Interface Section
// ==================================================
public:
	// Interface
	virtual void ApplyDamage(float DamageAmount) override;
	virtual void ApplyDamageWithKnockback(float DamageAmount, const FVector& KnockbackDirection, float KnockbackStrength) override;

protected:
	bool CanReceiveDamage();

public:
	FOnDeathDelegate OnDeathDelegate;

// ==================================================
// Inventory Test Section
// ==================================================
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDSWeaponItemData> DefaultWeaponItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDSWeaponItemData> TestWeaponItemData01;


// ==================================================
// UI Section
// ==================================================
public:
	FOnHealthChanged OnHealthChangedDelegate;

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
};
