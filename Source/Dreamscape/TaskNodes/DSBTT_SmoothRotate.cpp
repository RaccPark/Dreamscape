// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskNodes/DSBTT_SmoothRotate.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UDSBTT_SmoothRotate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}

void UDSBTT_SmoothRotate::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

	if (!Pawn || !Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 회전 로직
	FRotator Current = Pawn->GetActorRotation();
	FRotator TargetRotation = (Target->GetActorLocation() - Pawn->GetActorLocation()).Rotation();

	FRotator NewRotation = FMath::RInterpTo(Current, FRotator(0.f, TargetRotation.Yaw, 0.f), DeltaSeconds, 5.f);
	Pawn->SetActorRotation(NewRotation);

	// 회전 오차 보정
	float DeltaYaw = FMath::Abs(Current.Yaw - TargetRotation.Yaw);
	if (DeltaYaw < 5.f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
