// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskNodes/DSBTT_SetFocus.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UDSBTT_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
    if (Target)
    {
        AIController->SetFocus(Target);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
