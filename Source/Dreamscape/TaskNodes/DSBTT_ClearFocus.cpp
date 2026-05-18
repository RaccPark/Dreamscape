// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskNodes/DSBTT_ClearFocus.h"
#include "AIController.h"

EBTNodeResult::Type UDSBTT_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    AIController->ClearFocus(EAIFocusPriority::Gameplay);

    return EBTNodeResult::Succeeded;
}
