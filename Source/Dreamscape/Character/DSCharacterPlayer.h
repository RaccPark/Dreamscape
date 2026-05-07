// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DSCharacterBase.h"
#include "Interface/DSDamageableInterface.h"
#include "DSCharacterPlayer.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, int32, NewHelath);

UCLASS()
class DREAMSCAPE_API ADSCharacterPlayer : public ADSCharacterBase, public IDSDamageableInterface
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
	TObjectPtr<class UInputAction> LookAction;

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
	void Look(const struct FInputActionValue& Value);

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
	UCameraComponent* GetCamera();

	UDSPlayerFSMComponent* GetPlayerFSMComponent() const;

	class UInputMappingContext* GetDefaultMappingContext() const;

	class UDSInventoryComponent* GetInventoryComponent() const;

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

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	void SetComboCheckTimer();
	void ComboCheck();

	int CurrentCombo;

	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand;
public:
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
// Damageable Interface Section
// ==================================================
public:
	virtual void ApplyDamage(float DamageAmount) override;
	virtual void ApplyDamageWithKnockback(float DamageAmount, const FVector& KnockbackDirection, float KnockbackStrength) override;

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
};
