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

#include "Components/DSPlayerStatsComponent.h"
#include "Components/DSPlayerVitalityComponent.h"
#include "Components/DSPlayerCombatComponent.h"

#include "Animation/AnimMontage.h"

#include "Character/DSCharacterComboActionData.h"
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
	if (InputActionSwordAttackActionRef.Object)
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
	// 암 길이가 수천 유닛이라 회전 랙을 켜면 작은 각도 지연이 그대로 큰 위치 스윙이 된다.
	// Yaw는 UpdateCameraYaw()에서 직접 보간하므로 회전 랙은 끈다.
	CameraBoom->bEnableCameraRotationLag = false;
	CameraBoom->CameraLagSpeed = 3.0f;
	CameraBoom->CameraLagMaxDistance = 300.f;
	CameraBoom->ProbeSize = 8.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	// 실제 값은 DSC_Move(UDSCharacterControlData)에서 덮어쓴다. 기획 기준 FOV 4~10.
	FollowCamera->FieldOfView = 8.0f;

	// 카메라 회전 기믹용 Yaw 상태. 실제 시작값은 SetCharacterControlData()에서 데이터 에셋과 동기화된다.
	CameraYawInterpSpeed = 4.0f;
	CurrentCameraYaw = 0.0f;
	TargetCameraYaw = 0.0f;

	// Camera Peek Component
	CameraPeekComponent = CreateDefaultSubobject<UDSCameraPeekComponent>(TEXT("CameraPeekComponent"));

	// Player FSM Component
	PlayerFSMComponent = CreateDefaultSubobject<UDSPlayerFSMComponent>(TEXT("PlayerFSMComponent"));

	// Player Stats / Vitality / Combat Component
	StatsComponent = CreateDefaultSubobject<UDSPlayerStatsComponent>(TEXT("StatsComponent"));
	VitalityComponent = CreateDefaultSubobject<UDSPlayerVitalityComponent>(TEXT("VitalityComponent"));
	CombatComponent = CreateDefaultSubobject<UDSPlayerCombatComponent>(TEXT("CombatComponent"));

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
}

void ADSCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 스탯 -> 체력 순서로 초기화한다. Vitality가 Stats의 최대치를 읽어가기 때문에 순서가 중요하다.
	// BeginPlay가 아니라 여기여야 HUD 위젯이 붙기 전에 값이 확정된다.
	if (StatsComponent)
	{
		StatsComponent->InitializeStats();
	}

	if (VitalityComponent)
	{
		VitalityComponent->Initialize(StatsComponent);
	}
}

void ADSCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 체력 이벤트를 FSM으로 넘기는 배선. 컴포넌트가 FSM을 직접 부르지 않도록 캐릭터가 중재한다.
	if (VitalityComponent)
	{
		VitalityComponent->OnStaggered.AddDynamic(this, &ADSCharacterPlayer::HandleStaggered);
		VitalityComponent->OnDeath.AddDynamic(this, &ADSCharacterPlayer::HandleVitalityDeath);
	}

	// 업그레이드로 이동 속도가 바뀌면 다시 적용한다.
	if (StatsComponent)
	{
		StatsComponent->OnStatsChanged.AddDynamic(this, &ADSCharacterPlayer::ApplyMoveSpeedFromStats);
	}

	// 공격 시퀀스(콤보 + 회복)가 끝나면 FSM에 알린다. 컴포넌트는 FSM을 직접 부르지 않는다.
	if (CombatComponent)
	{
		CombatComponent->OnAttackSequenceEnded.AddDynamic(this, &ADSCharacterPlayer::HandleAttackSequenceEnded);
	}

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
			if (CombatComponent)
			{
				CombatComponent->EquipWeapon(DefaultWeaponItemData);
			}
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
	Super::Tick(DeltaTime);

	UpdateCameraYaw(DeltaTime);
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

	// 데이터 에셋이 지정한 Yaw를 카메라 회전 상태의 시작값으로 맞춘다.
	CurrentCameraYaw = TargetCameraYaw = FRotator::NormalizeAxis(CharacterControlData->RelativeRotation.Yaw);

	// Character Movement Settings
	this->MaxWalkSpeed = CharacterControlData->MaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = this->MaxWalkSpeed;

	// 데이터 에셋이 MaxWalkSpeed를 덮어쓴 뒤여야 스탯 배수가 살아남는다.
	ApplyMoveSpeedFromStats();
}

void ADSCharacterPlayer::ApplyMoveSpeedFromStats()
{
	if (!StatsComponent)
	{
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = this->MaxWalkSpeed * StatsComponent->GetMoveSpeedMultiplier();
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

	// 이동은 항상 "화면 기준 앞"(카메라 붐의 Yaw)을 기준으로 한다.
	// 맵 45도 회전 연출이나 추후 카메라 회전 기믹이 들어와도 조작 방향이 화면과 어긋나지 않게 하기 위함.
	const FRotationMatrix CameraYawMatrix(FRotator(0.0f, GetCameraYaw(), 0.0f));

	const FVector ForwardDirection = CameraYawMatrix.GetUnitAxis(EAxis::X);
	const FVector RightDirection = CameraYawMatrix.GetUnitAxis(EAxis::Y);

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

// ==================================================
// Camera Control Section
// ==================================================

UCameraComponent* ADSCharacterPlayer::GetCamera() const
{
	return FollowCamera;
}

USpringArmComponent* ADSCharacterPlayer::GetCameraBoom() const
{
	return CameraBoom;
}

float ADSCharacterPlayer::GetCameraYaw() const
{
	if (CameraBoom)
	{
		// GetTargetRotation()은 bUsePawnControlRotation / bInherit* 를 반영한, 랙이 걸리지 않은 목표 회전값이다.
		// 카메라가 부드럽게 도는 도중에도 조작 기준이 밀리지 않는다.
		return CameraBoom->GetTargetRotation().Yaw;
	}

	return Controller ? Controller->GetControlRotation().Yaw : GetActorRotation().Yaw;
}

void ADSCharacterPlayer::SetCameraYaw(float NewYaw, bool bInstant)
{
	TargetCameraYaw = FRotator::NormalizeAxis(NewYaw);

	if (bInstant && CameraBoom)
	{
		CurrentCameraYaw = TargetCameraYaw;

		FRotator BoomRotation = CameraBoom->GetRelativeRotation();
		BoomRotation.Yaw = CurrentCameraYaw;
		CameraBoom->SetRelativeRotation(BoomRotation);
	}
}

void ADSCharacterPlayer::AddCameraYaw(float DeltaYaw, bool bInstant)
{
	SetCameraYaw(TargetCameraYaw + DeltaYaw, bInstant);
}

void ADSCharacterPlayer::UpdateCameraYaw(float DeltaTime)
{
	if (!CameraBoom || CurrentCameraYaw == TargetCameraYaw)
	{
		return;
	}

	// 360도 경계에서 먼 쪽으로 도는 것을 막기 위해 각도 차이를 직접 구해 최단 방향으로 보간한다.
	const float DeltaAngle = FMath::FindDeltaAngleDegrees(CurrentCameraYaw, TargetCameraYaw);
	const float Alpha = FMath::Clamp(CameraYawInterpSpeed * DeltaTime, 0.0f, 1.0f);

	CurrentCameraYaw = (FMath::Abs(DeltaAngle) < 0.05f)
		? TargetCameraYaw
		: FRotator::NormalizeAxis(CurrentCameraYaw + DeltaAngle * Alpha);

	// bInheritYaw가 false이므로 상대 Yaw가 곧 실효 월드 Yaw가 된다.
	FRotator BoomRotation = CameraBoom->GetRelativeRotation();
	BoomRotation.Yaw = CurrentCameraYaw;
	CameraBoom->SetRelativeRotation(BoomRotation);
}

void ADSCharacterPlayer::DSCam(float NewFOV, float NewArmLength)
{
	if (!FollowCamera || !CameraBoom)
	{
		return;
	}

	if (NewFOV > 0.0f)
	{
		FollowCamera->SetFieldOfView(NewFOV);
	}

	if (NewArmLength > 0.0f)
	{
		CameraBoom->TargetArmLength = NewArmLength;
	}

	// UE의 FieldOfView는 수평 FOV이므로, 초점 평면에서 화면 가로로 보이는 월드 폭은 2 * Arm * tan(FOV/2).
	const float CurrentFOV = FollowCamera->FieldOfView;
	const float CurrentArmLength = CameraBoom->TargetArmLength;
	const float ViewWidth = 2.0f * CurrentArmLength * FMath::Tan(FMath::DegreesToRadians(CurrentFOV * 0.5f));

	UE_LOG(LogTemp, Warning, TEXT("[DSCam] FOV=%.2f, ArmLength=%.1f, ViewWidth=%.1f (ArmLength = (Width/2) / tan(FOV/2))"),
		CurrentFOV, CurrentArmLength, ViewWidth);
}

void ADSCharacterPlayer::DSCamYaw(float NewYaw)
{
	SetCameraYaw(NewYaw);

	UE_LOG(LogTemp, Warning, TEXT("[DSCam] CameraYaw %.2f -> %.2f (InterpSpeed=%.2f)"),
		CurrentCameraYaw, TargetCameraYaw, CameraYawInterpSpeed);
}

void ADSCharacterPlayer::DSCamRotate(float DeltaYaw)
{
	// 콘솔에서 인자를 생략하면 0으로 들어올 수 있으므로 기본 90도로 처리한다.
	const float AppliedDelta = FMath::IsNearlyZero(DeltaYaw) ? 90.0f : DeltaYaw;

	AddCameraYaw(AppliedDelta);

	UE_LOG(LogTemp, Warning, TEXT("[DSCam] CameraYaw %.2f -> %.2f (Delta=%.2f, InterpSpeed=%.2f). 이동 입력은 항상 화면 기준 앞(+X)이어야 한다."),
		CurrentCameraYaw, TargetCameraYaw, AppliedDelta, CameraYawInterpSpeed);
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

UDSPlayerStatsComponent* ADSCharacterPlayer::GetStatsComponent() const
{
	return StatsComponent;
}

UDSPlayerVitalityComponent* ADSCharacterPlayer::GetVitalityComponent() const
{
	return VitalityComponent;
}

UDSPlayerCombatComponent* ADSCharacterPlayer::GetCombatComponent() const
{
	return CombatComponent;
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
	UE_LOG(LogTemp, Warning, TEXT("Player has died!"));
}

void ADSCharacterPlayer::HandleStaggered()
{
	if (PlayerFSMComponent)
	{
		PlayerFSMComponent->HandleHit();
	}
}

void ADSCharacterPlayer::HandleVitalityDeath()
{
	if (PlayerFSMComponent)
	{
		PlayerFSMComponent->HandleDeath();
	}
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

void ADSCharacterPlayer::ApplyDamage(const FDSDamageInfo& DamageInfo)
{
	if (!VitalityComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ADSCharacterPlayer] VitalityComponent is nullptr!"));
		return;
	}

	// 경직·사망 전이는 VitalityComponent의 이벤트를 통해 일어난다.
	// 무적(구르기 등)으로 피해가 무시되었으면 넉백도 적용하지 않는다.
	if (VitalityComponent->ApplyDamage(DamageInfo) && DamageInfo.HasKnockback())
	{
		LaunchCharacter(DamageInfo.KnockbackDirection * DamageInfo.KnockbackStrength, true, true);
	}
}

void ADSCharacterPlayer::DSVital()
{
	if (!VitalityComponent || !StatsComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DSVital] Stats/Vitality 컴포넌트가 없습니다."));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[DSVital] 체력 %.0f/%.0f, 스테미나 %.2f/%.0f, 무적 %s, 사망 %s"),
		VitalityComponent->GetCurrentHealth(), VitalityComponent->GetMaxHealth(),
		VitalityComponent->GetCurrentStamina(), VitalityComponent->GetMaxStamina(),
		VitalityComponent->IsInvincible() ? TEXT("O") : TEXT("X"),
		VitalityComponent->IsDead() ? TEXT("O") : TEXT("X"));

	UE_LOG(LogTemp, Warning, TEXT("[DSVital] 공격력 %.2f, 보조 공격력 %.2f, 공격속도 x%.2f, 이동속도 x%.2f(=%.1f), 사거리 x%.2f, 구르기 쿨 %.2f초"),
		StatsComponent->GetAttackPower(), StatsComponent->GetSubAttackPower(),
		StatsComponent->GetAttackSpeedMultiplier(),
		StatsComponent->GetMoveSpeedMultiplier(), GetCharacterMovement()->MaxWalkSpeed,
		StatsComponent->GetAttackRangeMultiplier(), StatsComponent->GetRollCooldown());
}

void ADSCharacterPlayer::DSDamage(float Amount)
{
	// 무적 판정과 경직/사망 전이를 그대로 타도록 실제 피해 경로를 부른다.
	// 콘솔 명령이라 공격자는 없다
	ApplyDamage(FDSDamageInfo(Amount, EDSDamageType::EnemyAttack, nullptr));

	DSVital();
}

void ADSCharacterPlayer::DSStamina(float Amount)
{
	if (!VitalityComponent)
	{
		return;
	}

	const bool bConsumed = VitalityComponent->ConsumeStamina(Amount);
	UE_LOG(LogTemp, Warning, TEXT("[DSStamina] 소모 %.2f -> %s"), Amount, bConsumed ? TEXT("성공") : TEXT("실패(부족)"));

	DSVital();
}

void ADSCharacterPlayer::DSUpgrade(const FString& StatName, int32 Amount)
{
	if (!StatsComponent)
	{
		return;
	}

	EDSPlayerStatType StatType;
	if (StatName.Equals(TEXT("Attack"), ESearchCase::IgnoreCase))
	{
		StatType = EDSPlayerStatType::AttackPower;
	}
	else if (StatName.Equals(TEXT("SubAttack"), ESearchCase::IgnoreCase))
	{
		StatType = EDSPlayerStatType::SubAttackPower;
	}
	else if (StatName.Equals(TEXT("Speed"), ESearchCase::IgnoreCase))
	{
		StatType = EDSPlayerStatType::AttackSpeed;
	}
	else if (StatName.Equals(TEXT("Mobility"), ESearchCase::IgnoreCase))
	{
		StatType = EDSPlayerStatType::Mobility;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[DSUpgrade] 알 수 없는 스탯: %s (Attack / SubAttack / Speed / Mobility)"), *StatName);
		return;
	}

	const bool bApplied = StatsComponent->AddUpgradePoint(StatType, Amount);
	UE_LOG(LogTemp, Warning, TEXT("[DSUpgrade] %s +%d -> %s (현재 %d포인트)"),
		*StatName, Amount, bApplied ? TEXT("적용") : TEXT("실패(상한)"), StatsComponent->GetUpgradePoint(StatType));

	DSVital();
}

// ==================================================
// Attack Trace Interface — CombatComponent에 위임
// ==================================================

void ADSCharacterPlayer::StartAttackTrace()
{
	if (CombatComponent)
	{
		CombatComponent->StartAttackTrace();
	}
}

void ADSCharacterPlayer::PerformAttackTrace()
{
	if (CombatComponent)
	{
		CombatComponent->PerformAttackTrace();
	}
}

void ADSCharacterPlayer::EndAttackTrace()
{
	if (CombatComponent)
	{
		CombatComponent->EndAttackTrace();
	}
}

void ADSCharacterPlayer::HandleAttackSequenceEnded()
{
	if (PlayerFSMComponent)
	{
		PlayerFSMComponent->HandleComboActionEnd();
	}
}

// ==================================================
// Combat Tuning (PIE 콘솔 전용)
// ==================================================

void ADSCharacterPlayer::DSCombat()
{
	if (!CombatComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DSCombat] CombatComponent가 없습니다."));
		return;
	}

	const UDSWeaponItemData* WeaponData = CombatComponent->GetEquippedWeaponData();
	if (!WeaponData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DSCombat] 장착된 무기가 없습니다."));
		return;
	}

	const int32 ComboCount = WeaponData->ComboActionData ? static_cast<int32>(WeaponData->ComboActionData->MaxComboCount) : 0;

	UE_LOG(LogTemp, Warning, TEXT("[DSCombat] 무기 %s | 콤보 %d회 | 배수 x%.2f | 회복 %.2f초 | 런치 %.0f"),
		*WeaponData->GetName(), ComboCount, WeaponData->DamageMultiplier, WeaponData->RecoveryTime, WeaponData->LungeImpulse);

	UE_LOG(LogTemp, Warning, TEXT("[DSCombat] 최종 피해 %.2f | 배속 x%.2f | 최종 회복 %.2f초 | 회복 중 %s"),
		CombatComponent->CalculateDamage(), CombatComponent->GetAttackPlayRate(),
		CombatComponent->GetRecoveryTime(),
		CombatComponent->IsRecovering() ? TEXT("O") : TEXT("X"));

	// 콤보별 선입력 창이 열리는 시각 (현재 배속 기준). 섹션 길이보다 커지면 콤보가 끊긴다.
	FString WindowLog;
	for (int32 Combo = 1; Combo <= ComboCount; ++Combo)
	{
		const float OpenTime = CombatComponent->GetComboWindowOpenTime(Combo);
		WindowLog += FString::Printf(TEXT("%d타 %.2fs  "), Combo, OpenTime);
	}
	UE_LOG(LogTemp, Warning, TEXT("[DSCombat] 선입력 창 열림: %s"), *WindowLog);
}

void ADSCharacterPlayer::DSWeaponTrace(int32 bEnabled)
{
	if (!CombatComponent)
	{
		return;
	}

	CombatComponent->SetDrawWeaponTrace(bEnabled != 0);
	UE_LOG(LogTemp, Warning, TEXT("[DSWeaponTrace] 무기 트레이스 드로우 %s"), bEnabled != 0 ? TEXT("ON") : TEXT("OFF"));
}
