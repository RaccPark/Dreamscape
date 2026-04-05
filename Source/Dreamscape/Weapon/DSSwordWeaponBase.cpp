// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/DSSwordWeaponBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/DSCharacterPlayer.h"
#include "DrawDebugHelpers.h"
#include "Interface/DSDamageableInterface.h"

// Sets default values
ADSSwordWeaponBase::ADSSwordWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SwordWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordWeaponMesh"));
	SetRootComponent(SwordWeaponMesh);
	SwordWeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));

	bTraceAtive = false;
}

// Called when the game starts or when spawned
void ADSSwordWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADSSwordWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bTraceAtive)
	{
		PerformTrace();
	}
}

void ADSSwordWeaponBase::StartAttackTrace()
{
	bTraceAtive = true;
	HitActors.Empty();
}


void ADSSwordWeaponBase::EndAttackTrace()
{
	bTraceAtive = false;
}

void ADSSwordWeaponBase::PerformTrace()
{
	if (!SwordWeaponMesh->DoesSocketExist("Start"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sockets 'Start' do not exist on the sword mesh!"));
	}

	if (!SwordWeaponMesh->DoesSocketExist("End"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sockets 'End' do not exist on the sword mesh!"));
	}

	FVector Start = SwordWeaponMesh->GetSocketLocation("Start");
	FVector End = SwordWeaponMesh->GetSocketLocation("End");

	DrawDebugSphere(GetWorld(), Start, 5.f, 12, FColor::Green, false, 2.f);
	DrawDebugSphere(GetWorld(), End, 5.f, 12, FColor::Blue, false, 2.f);

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f, 0, 2.f);

	TArray<FHitResult> Hits;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Start, End, 1.f, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, HitActors, EDrawDebugTrace::None, Hits, true);

	for (auto& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();

		if (HitActor && HitActor->IsA(ADSCharacterPlayer::StaticClass()))
		{
			continue; // Skip player character
		}

		if (HitActor && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());

			// Deal damage to the hit actor or apply any other effects here
			IDSDamageableInterface* DamageableActor = Cast<IDSDamageableInterface>(HitActor);
			if (DamageableActor)
			{
				// Example damage application
				FVector KnockbackDirection = (HitActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				FVector KnockbackNoHeightDirection = FVector(KnockbackDirection.X, KnockbackDirection.Y, 0.f).GetSafeNormal();
				DamageableActor->ApplyDamageWithKnockback(1.f, KnockbackNoHeightDirection, 250.f); // Example knockback
			}
		}
	}
}

