// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DSCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DSCharacterControlData.h"
#include "Components/DSCameraPeekComponent.h"

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
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Input/Actions/IA_Jump.IA_Jump'"));
	if (InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
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
	
	// Player Movement Settings
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->GravityScale = 1.6f;
	DefaultMaxWalkSpeed = 225.0f;
	this->MaxWalkSpeed = DefaultMaxWalkSpeed;						// Setting Default Max Walk Speed
	GetCharacterMovement()->MaxWalkSpeed = this->MaxWalkSpeed;		// Apply Default Max Walk Speed
	GetCharacterMovement()->MaxStepHeight = 5.0f;
	GetCharacterMovement()->SetWalkableFloorAngle(50.f);

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	bUseControllerRotationYaw = false;
}

void ADSCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	SetCharacterControl(ECharacterControlType::Quarter);
}

void ADSCharacterPlayer::Tick(float DeltaTime)
{
}

void ADSCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// Jump
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	// Move
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADSCharacterPlayer::Move);
	// Look
	//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADSCharacterPlayer::Look);
	// Camera Peek
	EnhancedInputComponent->BindAction(CameraPeekAction, ETriggerEvent::Started, this, &ADSCharacterPlayer::OnPeekStarted);
	EnhancedInputComponent->BindAction(CameraPeekAction, ETriggerEvent::Completed, this, &ADSCharacterPlayer::OnPeekEnded);
	EnhancedInputComponent->BindAction(MousePosition, ETriggerEvent::Triggered, this, &ADSCharacterPlayer::OnMouseInput);
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

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	//CameraBoom->SetRelativeLocation(CharacterControlData->RelativeLocation);
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;

	FollowCamera->FieldOfView = CharacterControlData->FOV;
}

void ADSCharacterPlayer::Move(const FInputActionValue& Value)
{
	/*
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (MovementVector.IsNearlyZero()) return;

	const FVector Forward = FVector(1.f, 1.f, 0.f).GetSafeNormal();
	const FVector Right = FVector(-1.f, 1.f, 0.f).GetSafeNormal();

	AddMovementInput(Forward, MovementVector.X);
	AddMovementInput(Right, MovementVector.Y);

	// Rotation
	FVector MoveDirection = Forward * MovementVector.X + Right * MovementVector.Y;

	if (!MoveDirection.IsNearlyZero())
	{
		MoveDirection.Normalize();
		FRotator TargetRotation = MoveDirection.Rotation();

		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 10.0f);

		SetActorRotation(NewRotation);
	}
	*/

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

void ADSCharacterPlayer::OnPeekStarted(const FInputActionValue& Value)
{
	if (CameraPeekComponent)
	{
		CameraPeekComponent->SetPeekActive(true);
	}
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
	if (!CameraPeekComponent)
	{
		return;
	}

	const FVector2D MouseDelta = Value.Get<FVector2D>();
	CameraPeekComponent->AddMouseDelta(MouseDelta);
}
