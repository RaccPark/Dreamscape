// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DSCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

#include "GameMode/DSPlayerController.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DSCharacterControlData.h"

#include "Components/DSCameraPeekComponent.h"
#include "Components/DSPlayerFSMComponent.h"
#include "Components/PlayerStateBase/DSPlayerStateBase.h"

#include "Animation/AnimMontage.h"

#include "Character/DSCharacterComboActionData.h"
#include "Weapon/DSSwordWeaponBase.h"
#include "Weapon/Data/DSWeaponItemData.h"

#include "Components/DSInventoryComponent.h"

#include "Components/DSInteractionComponent.h"

#include "Subsystem/DSTutorialSubsystem.h"

ADSCharacterPlayer::ADSCharacterPlayer()
{
	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Character/Input/IMC_Default.IMC_Default'"));
	if (DefaultMappingContextRef.Object)
	{
		DefaultMappingContext = DefaultMappingContextRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Input/Actions/IA_Move.IA_Move'"));
	if (InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Input/Actions/IA_Look.IA_Look'"));
	if (InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRollRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Input/Actions/IA_Roll.IA_Roll'"));
	if (InputActionRollRef.Object)
	{
		RollAction = InputActionRollRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionPeekRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Input/Actions/IA_CameraPeek.IA_CameraPeek'"));
	if (InputActionPeekRef.Object)
	{
		CameraPeekAction = InputActionPeekRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMousePositionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Input/Actions/IA_MousePosition.IA_MousePosition'"));
	if (InputActionMousePositionRef.Object)
	{
		MousePosition = InputActionMousePositionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSwordAttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Input/Actions/IA_SwordAttack.IA_SwordAttack'"));
	if (InputActionMousePositionRef.Object)
	{
		SwordAttackAction = InputActionSwordAttackActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionPauseActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Input/Actions/IA_Pause.IA_Pause'"));
	if (InputActionPauseActionRef.Object)
	{
		PauseAction = InputActionPauseActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionInteractActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Input/Actions/IA_Interact.IA_Interact'"));
	if (InputActionInteractActionRef.Object)
	{
		InteractAction = InputActionInteractActionRef.Object;
	}

	// Default Mesh & Animation Setting
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultSkeletonMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Fab/Free_Animated_Low_Poly_Cartoon_Skeleton/free_animated_low_poly_cartoon_skeleton.free_animated_low_poly_cartoon_skeleton'"));
	if (DefaultSkeletonMeshRef.Object)
	{
		DefaultSkeletonMesh = DefaultSkeletonMeshRef.Object;
		GetMesh()->SetSkeletalMesh(DefaultSkeletonMesh);
	}
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// Initialize Character Control Data
	// After Startup, you can edit these data assets in the editor
	// Here is just setting default data assets
	
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 900.0f;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 15.0f));
	//CameraBoom->SetRelativeRotation(FRotator(0.0f, -40.0f, 45.0f));	// Don't use
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagSpeed = 3.0f;
	CameraBoom->CameraRotationLagSpeed = 20.0f;
	CameraBoom->CameraLagMaxDistance = 300.f;
	CameraBoom->ProbeSize = 8.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->FieldOfView = 90.0f;

	// Camera Peek Component
	CameraPeekComponent = CreateDefaultSubobject<UDSCameraPeekComponent>(TEXT("CameraPeekComponent"));

	// Player FSM Component
	PlayerFSMComponent = CreateDefaultSubobject<UDSPlayerFSMComponent>(TEXT("PlayerFSMComponent"));

	// Player Inventory Component
	InventoryComponent = CreateDefaultSubobject<UDSInventoryComponent>(TEXT("InventoryComponent"));

	// Player Interaction Component
	InteractionComponent = CreateDefaultSubobject<UDSInteractionComponent>(TEXT("InteractionComponent"));
	
	// Player Movement Settings
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->GravityScale = 1.6f;
	DefaultMaxWalkSpeed = 225.0f;
	this->MaxWalkSpeed = DefaultMaxWalkSpeed;						// Setting Default Max Walk Speed
	GetCharacterMovement()->MaxWalkSpeed = this->MaxWalkSpeed;		// Apply Default Max Walk Speed
	GetCharacterMovement()->MaxStepHeight = 5.0f;
	GetCharacterMovement()->SetWalkableFloorAngle(50.f);
	TurnInterpSpeed = 15.0f;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	bUseControllerRotationYaw = false;

	CurrentCombo = 0;

	bIsInvincible = false;
}

void ADSCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->SetShowMouseCursor(true);
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	GetMesh()->GetAnimInstance()->RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;

	SetCharacterControl(ECharacterControlType::Quarter);

	// Inventory Test
	if (DefaultWeaponItemData && InventoryComponent)
	{
		bool bResult = InventoryComponent->AddItem(DefaultWeaponItemData, 1);
		//InventoryComponent->AddItem(TestWeaponItemData01, 1);

		if (bResult)
		{
			EquipSwordWeapon(DefaultWeaponItemData);
			UE_LOG(LogTemp, Warning, TEXT("Item added to inventory successfully."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to add item to inventory."));
		}
	}

	// 튜토리얼 시작
	APlayerController* PC = CastChecked<APlayerController>(GetController());
	if (UDSTutorialSubsystem* TutorialSubsystem = ULocalPlayer::GetSubsystem<UDSTutorialSubsystem>(PC->GetLocalPlayer()))
	{
		TutorialSubsystem->BindAndStart(this);
	}

}

void ADSCharacterPlayer::Tick(float DeltaTime)
{
}

void ADSCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// Move
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADSCharacterPlayer::HandleMove);
	// Roll
	EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &ADSCharacterPlayer::StartRoll);

	// Camera Peek
	EnhancedInputComponent->BindAction(CameraPeekAction, ETriggerEvent::Started, this, &ADSCharacterPlayer::OnPeekStarted);
	EnhancedInputComponent->BindAction(CameraPeekAction, ETriggerEvent::Completed, this, &ADSCharacterPlayer::OnPeekEnded);
	EnhancedInputComponent->BindAction(MousePosition, ETriggerEvent::Triggered, this, &ADSCharacterPlayer::OnMouseInput);

	// Attack
	EnhancedInputComponent->BindAction(SwordAttackAction, ETriggerEvent::Started, this, &ADSCharacterPlayer::SwordAttack);

	// Pause(Open Menu)
	EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ADSCharacterPlayer::OnPausePressed);

	// Interact
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ADSCharacterPlayer::OnInteractPressed);
}

void ADSCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UDSCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void ADSCharacterPlayer::SetCharacterControlData(const UDSCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	// Camera Settings
	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	//CameraBoom->SetRelativeLocation(CharacterControlData->RelativeLocation);
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;

	FollowCamera->FieldOfView = CharacterControlData->FOV;

	// Character Movement Settings
	this->MaxWalkSpeed = CharacterControlData->MaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = this->MaxWalkSpeed;
}

void ADSCharacterPlayer::HandleMove(const struct FInputActionValue& Value)
{
	if (PlayerFSMComponent)
	{
		PlayerFSMComponent->HandleMoveInput(Value);
	}
}

void ADSCharacterPlayer::Move(const FInputActionValue& Value)
{
	// 이동 설정
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);

	// Rotation
	if (!MovementVector.IsNearlyZero())
	{
		FVector MoveDirection = (ForwardDirection * MovementVector.X) + (RightDirection * MovementVector.Y);
		MoveDirection.Z = 0.f;

		FRotator TargetRotation = MoveDirection.Rotation();
		FRotator CurrentRotation = GetActorRotation();

		FRotator NewRotation = FMath::RInterpTo(
			CurrentRotation,
			TargetRotation,
			GetWorld()->GetDeltaSeconds(),
			TurnInterpSpeed
		);

		SetActorRotation(NewRotation);
	}
}

void ADSCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// Set Custom Mouse Sensitivity Coefficient.
	// Because Unreal Default Mouse Sensitivity is too high.
	float MouseSensitivityCoefficient = 0.05f;

	// Multiply MouseSensivity
	float ManipulatedMouseSensitivity = 1.0f * MouseSensitivityCoefficient;

	// Apply Mouse Sensitivity to LookAxisVector
	LookAxisVector *= ManipulatedMouseSensitivity;

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ADSCharacterPlayer::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		if (PlayerFSMComponent)
		{
			PlayerFSMComponent->HandleFall();
		}
	}
	else if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking && PrevMovementMode == EMovementMode::MOVE_Falling)
	{
		if (PlayerFSMComponent)
		{
			PlayerFSMComponent->HandleLand();
		}
	}
}

void ADSCharacterPlayer::StartRoll(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Start Roll"));

	if (PlayerFSMComponent)
	{
		PlayerFSMComponent->HandleRollInput();
	}
}

void ADSCharacterPlayer::OnRollEnd()
{
	if (PlayerFSMComponent)
	{
		PlayerFSMComponent->HandleRollEnd();
	}
}

void ADSCharacterPlayer::StartFall()
{

}

void ADSCharacterPlayer::StopFall()
{

}

void ADSCharacterPlayer::OnPeekStarted(const FInputActionValue& Value)
{
	if (CameraPeekComponent)
	{
		CameraPeekComponent->SetPeekActive(true);
	}

	OnCameraPeekStartedDelegate.Broadcast();
}

void ADSCharacterPlayer::OnPeekEnded(const FInputActionValue& Value)
{
	if (CameraPeekComponent)
	{
		CameraPeekComponent->SetPeekActive(false);
	}
}

void ADSCharacterPlayer::OnMouseInput(const FInputActionValue& Value)
{
	if (CameraPeekComponent)
	{
		const FVector2D MouseDelta = Value.Get<FVector2D>();
		CameraPeekComponent->AddMouseDelta(MouseDelta);
	}
}

UDSPlayerFSMComponent* ADSCharacterPlayer::GetPlayerFSMComponent() const
{
	if (!PlayerFSMComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerFSMComponent is nullptr!"));
		return nullptr;
	}

	return PlayerFSMComponent;
}

UInputMappingContext* ADSCharacterPlayer::GetDefaultMappingContext() const
{
	return DefaultMappingContext;
}

UDSInventoryComponent* ADSCharacterPlayer::GetInventoryComponent() const
{
	return InventoryComponent;
}

UDSInteractionComponent* ADSCharacterPlayer::GetInteractionComponent() const
{
	return InteractionComponent;
}

void ADSCharacterPlayer::OnPausePressed(const FInputActionValue& Value)
{
	ADSPlayerController* PlayerController = Cast<ADSPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->ChangeInputMode(EPlayerInputMode::UI);
	}
}

void ADSCharacterPlayer::OnInteractPressed(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Interact Pressed"));
	if (InteractionComponent)
	{
		UE_LOG(LogTemp, Log, TEXT("Trying to interact..."));
		InteractionComponent->TryInteract();
	}
}

void ADSCharacterPlayer::SwordAttack(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Sword Attack"));

	if (PlayerFSMComponent)
	{
		PlayerFSMComponent->HandleSwordAttackInput();
	}
}

void ADSCharacterPlayer::OnDeath()
{
	if (OnDeathDelegate.IsBound())
	{
		OnDeathDelegate.Broadcast();
	}

	UE_LOG(LogTemp, Warning, TEXT("Player has died!"));
}

void ADSCharacterPlayer::PlayRollMontage()
{
	if (!RollMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("RollMontage is not set!"));
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimInstance is not valid!"));
		return;
	}

	AnimInstance->Montage_Play(RollMontage);
}

void ADSCharacterPlayer::PlayHitMontage()
{
	if (!HitMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("HitMontage is not set!"));
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimInstance is not valid!"));
		return;
	}
	AnimInstance->Montage_Play(HitMontage);
}

void ADSCharacterPlayer::RotateCharacterToMouseCursor()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is not valid!"));
		return;
	}

	FVector WorldLocation;
	FVector WorldDirection;

	bool bDeprojected = PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	if (bDeprojected)
	{
		// 바닥 평면 (캐릭터 높이 기준)
		float PlaneZ = GetActorLocation().Z;

		// Ray vs Plane 교차 계산
		float T = (PlaneZ - WorldLocation.Z) / WorldDirection.Z;
		FVector HitPoint = WorldLocation + (WorldDirection * T);

		FVector LookAtVector = HitPoint - GetActorLocation();
		LookAtVector.Z = 0.f;

		if (!LookAtVector.IsNearlyZero())
		{
			FRotator TargetRotation = FRotationMatrix::MakeFromX(LookAtVector).Rotator();
			SetActorRotation(TargetRotation);
		}
	}
}

void ADSCharacterPlayer::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		// 즉시 실행 — Command 생성 후 바로 Execute (PDF 구현방법1 구조)
		UE_LOG(LogTemp, Log, TEXT("[Command] ProcessComboCommand: 첫 공격 -> DSSwordAttackCommand 생성 후 즉시 Execute"));
		TUniquePtr<IDSComboCommand> Cmd = MakeUnique<DSSwordAttackCommand>();
		Cmd->Execute(this);
		return;
	}

	if (ComboTimerHandle.IsValid())
	{
		// 버퍼에 적재 — 타이머 만료 시점에 실행 (PDF 구현방법2 구조)
		UE_LOG(LogTemp, Log, TEXT("[Command] ProcessComboCommand: 콤보 %d 진행 중 -> PendingComboCommand 적재"), CurrentCombo);
		PendingComboCommand = MakeUnique<DSSwordAttackCommand>();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Command] ProcessComboCommand: 타이머 만료 후 입력 -> 커맨드 버림 (유효 타이밍 아님)"));
	}
}

void ADSCharacterPlayer::EquipSwordWeapon(const UDSWeaponItemData* NewWeaponData)
{
	if (!NewWeaponData)
	{
		UE_LOG(LogTemp, Warning, TEXT("NewWeaponData is not set!"));
		return;
	}

	// 기존 무기 해제
	if (EquippedSwordWeapon)
	{
		EquippedSwordWeapon->Destroy();
		EquippedSwordWeapon = nullptr;
	}

	// 새로운 무기 장착
	FName SocketName = TEXT("WeaponIdleSocket");	// 등 뒤 장착(무기 Idle)
	FVector SpawnLocation = GetMesh()->GetSocketLocation(SocketName);
	FRotator SpawnRotation = GetMesh()->GetSocketRotation(SocketName);

	// Spawn 정보 구체화
	// SpawnActor()의 호출 시 생성 동작을 세밀하게 제어하기 위한 구조체
	// Owner과 Istigator(행동의 주체) 설정이 필요할 때 사용
	// 데미지 전달, 팀 판정, AI 인식 등에 중요
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	ADSSwordWeaponBase* SpawnedWeapon = GetWorld()->SpawnActor<ADSSwordWeaponBase>(NewWeaponData->SwordWeaponClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (SpawnedWeapon)
	{
		SpawnedWeapon->InitializeWeapon(NewWeaponData);

		SpawnedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}

	EquippedSwordWeapon = SpawnedWeapon;
	UE_LOG(LogTemp, Log, TEXT("Equipped new sword weapon: %s"), *NewWeaponData->GetName());
}

void ADSCharacterPlayer::SetEquippedWeaponSocket(FName SocketName)
{
	if (!EquippedSwordWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("EquippedSwordWeapon is not set!"));
		return;
	}
	// 소켓에서 장착 해제
	EquippedSwordWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	EquippedSwordWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

ADSSwordWeaponBase* ADSCharacterPlayer::GetEquippedSwordWeapon() const
{
	if(!EquippedSwordWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("EquippedSwordWeapon is not set!"));
		return nullptr;
	}
	return EquippedSwordWeapon;
}

void ADSCharacterPlayer::ApplyDamage(float DamageAmount)
{
	if (!CanReceiveDamage())
	{
		return;
	}

	CurrentHealth -= DamageAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
	OnHealthChangedDelegate.Broadcast(CurrentHealth);

	if (CurrentHealth <= 0.0f)
	{
		PlayerFSMComponent->HandleDeath();
		return;
	}

	PlayerFSMComponent->HandleHit();

	UE_LOG(LogTemp, Log, TEXT("ApplyDamage: %f"), DamageAmount);
}

void ADSCharacterPlayer::ApplyDamageWithKnockback(float DamageAmount, const FVector& KnockbackDirection, float KnockbackStrength)
{
	ApplyDamage(DamageAmount);
	LaunchCharacter(KnockbackDirection * KnockbackStrength, true, true);
}

bool ADSCharacterPlayer::CanReceiveDamage()
{
	if (!PlayerFSMComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ADSCharacterPlayer] PlayerFSMComponent is nullptr!"));
		return false;
	}

	EPlayerStateType CurrentState = PlayerFSMComponent->GetCurrentStateType();

	if (CurrentState == EPlayerStateType::EPS_Roll || CurrentState == EPlayerStateType::EPS_Hit || CurrentState == EPlayerStateType::EPS_Death)
	{
		return false;
	}

	return true;
}

void ADSCharacterPlayer::StartAttackTrace()
{
	if (GetEquippedSwordWeapon())
	{
		EquippedSwordWeapon->StartAttackTrace();
		// 임시 캐릭터 런치
		LaunchCharacter(GetActorForwardVector() * 600.f, true, true);
	}
}

void ADSCharacterPlayer::PerformAttackTrace()
{
	if (GetEquippedSwordWeapon())
	{
		EquippedSwordWeapon->PerformTrace();
	}
}

void ADSCharacterPlayer::EndAttackTrace()
{
	if (GetEquippedSwordWeapon())
	{
		EquippedSwordWeapon->EndAttackTrace();
	}
}

void ADSCharacterPlayer::ComboActionBegin()
{
	CurrentCombo = 1;
	
	const float AttackSpeedRate = 1.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimInstance is not valid!"));
		return;
	}
	AnimInstance->Montage_Play(SwordAttackMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ADSCharacterPlayer::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, SwordAttackMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void ADSCharacterPlayer::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	//PlayerFSMComponent->ChangeState(EPlayerStateType::EPS_Idle);
	PlayerFSMComponent->HandleComboActionEnd();
}

void ADSCharacterPlayer::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCounts.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f;
	// 발동된 시간을 알기 위하여 ComboActionData에서 EffectiveFrameCounts와 FrameRate를 이용하여 계산
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCounts[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ADSCharacterPlayer::ComboCheck, ComboEffectiveTime, false);
	}
}

void ADSCharacterPlayer::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (PendingComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		UE_LOG(LogTemp, Log, TEXT("[Command] ComboCheck: PendingComboCommand 소비 -> 콤보 %d 섹션으로 점프"), CurrentCombo);

		FName NextSectionName = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNameProfix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSectionName, SwordAttackMontage);

		SetComboCheckTimer();
		PendingComboCommand.Reset();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[Command] ComboCheck: PendingComboCommand 없음 -> 콤보 종료 대기"));
	}
}
