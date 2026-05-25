// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/DSEnemyCharacterBase.h"
#include "Animation/DSEnemyAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/DSEnemyAnimInstance.h"

// Sets default values
ADSEnemyCharacterBase::ADSEnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 3.0f;
	CurrentHealth = MaxHealth;
	ChaseStartRange = 300.0f;
	AttackStartRange = 50.0f;
}

// Called when the game starts or when spawned
void ADSEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	EnemyAnimInstance = Cast<UDSEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	CurrentHealth = MaxHealth;

	
}

void ADSEnemyCharacterBase::Attack()
{

}

void ADSEnemyCharacterBase::OnDeath()
{
	// Disable character movement & collision
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DoRagdoll();

	// Set death flag in animation instance
	if (EnemyAnimInstance)
	{
		EnemyAnimInstance->bIsDead = true;
	}
}

void ADSEnemyCharacterBase::DoRagdoll()
{
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

void ADSEnemyCharacterBase::OnStateChanged(EDSEnemyState NewState)
{
	switch (NewState)
	{
	case EDSEnemyState::ES_Idle:
		break;
	case EDSEnemyState::ES_Chase:
		break;
	case EDSEnemyState::ES_Attack:
		break;
	case EDSEnemyState::ES_Dead:
		break;
	default:
		break;
	}
}

void ADSEnemyCharacterBase::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// Handle logic after attack montage ends, e.g., reset attack state
	if (OnAttackEnded.IsBound())
	{
		OnAttackEnded.Broadcast();
	}
}

// Called every frame
void ADSEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADSEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ADSEnemyCharacterBase::GetChaseStartRange() const
{
	return ChaseStartRange;
}

float ADSEnemyCharacterBase::GetAttackStartRange() const
{
	return AttackStartRange;
}

EDSEnemyState ADSEnemyCharacterBase::GetState() const
{
	return CurrentState;
}

void ADSEnemyCharacterBase::SetState(EDSEnemyState NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	CurrentState = NewState;
}

void ADSEnemyCharacterBase::PlayLandingSequnce()
{
	
}

