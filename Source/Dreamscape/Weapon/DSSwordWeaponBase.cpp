// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/DSSwordWeaponBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/DSDamageableInterface.h"
#include "Weapon/Data/DSWeaponItemData.h"

ADSSwordWeaponBase::ADSSwordWeaponBase()
{
	// 판정은 소유자가 애님 노티파이 타이밍에 직접 부른다. 자체 틱은 필요 없다.
	PrimaryActorTick.bCanEverTick = false;

	SwordWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordWeaponMesh"));
	SetRootComponent(SwordWeaponMesh);
	SwordWeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));

	// 종전 하드코딩 값 그대로
	TraceRadius = 12.0f;
	TraceHalfHeight = 30.0f;
	bDrawTrace = false;

	DamageAmount = 0.0f;
}

void ADSSwordWeaponBase::InitializeWeapon(const UDSWeaponItemData* WeaponData)
{
	if (WeaponData)
	{
		DamageAmount = WeaponData->DamageAmount;
		UE_LOG(LogTemp, Log, TEXT("[ADSSwordWeaponBase] 초기화: %s"), *WeaponData->GetName());
	}
}

// ==================================================
// 판정
// ==================================================

void ADSSwordWeaponBase::StartAttackTrace()
{
	HitActors.Empty();
}

void ADSSwordWeaponBase::EndAttackTrace()
{
	HitActors.Empty();
}

void ADSSwordWeaponBase::CollectHits(TArray<AActor*>& OutNewHits)
{
	if (!SwordWeaponMesh->DoesSocketExist("Start") || !SwordWeaponMesh->DoesSocketExist("End"))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ADSSwordWeaponBase] 무기 메시에 Start/End 소켓이 없습니다."));
		return;
	}

	const FVector Start = SwordWeaponMesh->GetSocketLocation("Start");
	const FVector End = SwordWeaponMesh->GetSocketLocation("End");

	TArray<FHitResult> Hits;
	UKismetSystemLibrary::CapsuleTraceMulti(
		GetWorld(), Start, End, TraceRadius, TraceHalfHeight,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
		false, HitActors,
		bDrawTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		Hits, true);

	AActor* OwnerActor = GetOwner();

	// 한 스윙에 같은 액터는 한 번만. 칼이 지나가는 순서대로 쌓인다.
	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor || HitActor == OwnerActor || HitActors.Contains(HitActor))
		{
			continue;
		}

		HitActors.Add(HitActor);
		OutNewHits.Add(HitActor);
	}
}

void ADSSwordWeaponBase::PerformTrace()
{
	TArray<AActor*> NewHits;
	CollectHits(NewHits);

	for (AActor* HitActor : NewHits)
	{
		if (IDSDamageableInterface* Damageable = Cast<IDSDamageableInterface>(HitActor))
		{
			FVector KnockbackDirection = HitActor->GetActorLocation() - GetActorLocation();
			KnockbackDirection.Z = 0.f;
			Damageable->ApplyDamageWithKnockback(DamageAmount, EDSDamageType::EnemyAttack, KnockbackDirection.GetSafeNormal(), 250.f);
		}
	}
}
