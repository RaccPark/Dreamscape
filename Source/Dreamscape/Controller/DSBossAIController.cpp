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

	if (!BehaviorTree)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DSBossAIController] BehaviorTree not assigned in %s"), *GetName());
	}

	// 이제 Boss Character 자체에서 StartAI로 실행
	/*RunBehaviorTree(BehaviorTree);

	BBComponent = GetBlackboardComponent();

	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (BBComponent)
	{
		TrySetPlayerAsTarget();
	}*/
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

void ADSBossAIController::SetBehaviorTreeByOwnController()
{
	RunBehaviorTree(BehaviorTree);

	BBComponent = GetBlackboardComponent();

	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (BBComponent)
	{
		TrySetPlayerAsTarget();
	}
}

UBehaviorTree* ADSBossAIController::GetBehaviorTree() const
{
	return BehaviorTree;
}
