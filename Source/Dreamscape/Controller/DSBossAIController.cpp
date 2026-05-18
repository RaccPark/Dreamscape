// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/DSBossAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ADSBossAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ADSBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);

		BBComponent = GetBlackboardComponent();

		APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
		if (BBComponent)
		{
			TrySetPlayerAsTarget();
		}
	}
}

void ADSBossAIController::Tick(float DeltaTime)
{

}

void ADSBossAIController::TrySetPlayerAsTarget()
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player found, setting as target"));
		BBComponent->SetValueAsObject("TargetActor", Player);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ADSBossAIController::TrySetPlayerAsTarget);
	}
}
