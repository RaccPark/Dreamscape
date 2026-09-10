// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/DSEnemyCharacterBase.h"
#include "Animation/DSEnemyAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/DSEnemyAnimInstance.h"

#include "Components/DSCharacterStatsComponent.h"
#include "Components/DSVitalityComponent.h"

// Sets default values
ADSEnemyCharacterBase::ADSEnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 체력은 컴포넌트가 소유한다. 최대 체력은 StatsComponent의 BaseMaxHealth(기본 3).
	StatsComponent = CreateDefaultSubobject<UDSCharacterStatsComponent>(TEXT("StatsComponent"));
	VitalityComponent = CreateDefaultSubobject<UDSVitalityComponent>(TEXT("VitalityComponent"));

	ChaseStartRange = 300.0f;
	AttackStartRange = 50.0f;
}

void ADSEnemyCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 스탯 -> 체력 순서. Vitality가 Stats의 최대치를 읽어가므로 순서가 중요하다.
	// ADSCharacterPlayer와 같은 흐름이다.
	if (StatsComponent)
	{
		StatsComponent->InitializeStats();
	}

	if (VitalityComponent)
	{
		VitalityComponent->Initialize(StatsComponent);
	}
}

// Called when the game starts or when spawned
void ADSEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	EnemyAnimInstance = Cast<UDSEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	if (VitalityComponent)
	{
		VitalityComponent->OnDamaged.AddDynamic(this, &ADSEnemyCharacterBase::HandleDamaged);
		VitalityComponent->OnDeath.AddDynamic(this, &ADSEnemyCharacterBase::HandleVitalityDeath);
	}
}

void ADSEnemyCharacterBase::ApplyDamage(float DamageAmount)
{
	if (!VitalityComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ADSEnemyCharacterBase] VitalityComponent is nullptr!"));
		return;
	}

	// 히트 리액션과 사망 처리는 VitalityComponent의 이벤트를 통해 일어난다.
	VitalityComponent->ApplyDamage(DamageAmount, this);
}

void ADSEnemyCharacterBase::ApplyDamageWithKnockback(float DamageAmount, const FVector& KnockbackDirection, float KnockbackStrength)
{
	if (!VitalityComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ADSEnemyCharacterBase] VitalityComponent is nullptr!"));
		return;
	}

	// 피해가 무시되었으면 넉백도 적용하지 않는다. 플레이어와 같은 규칙.
	// 넉백 저항치는 EnemyStatsComponent를 만들 때 얹는다.
	if (VitalityComponent->ApplyDamage(DamageAmount, this))
	{
		LaunchCharacter(KnockbackDirection * KnockbackStrength, true, true);
	}
}

void ADSEnemyCharacterBase::HandleDamaged(float DamageAmount, AActor* DamageInstigator)
{
	OnDamaged(DamageAmount);
}

void ADSEnemyCharacterBase::HandleVitalityDeath()
{
	OnDeath();
}

void ADSEnemyCharacterBase::OnDamaged(float DamageAmount)
{
	// 베이스는 아무것도 하지 않는다. 파생이 히트 리액션을 정한다.
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

