// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_ClearBlackboardValue.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyBTTask_ClearBlackboardValue::UMyBTTask_ClearBlackboardValue()
{
    NodeName = TEXT("Clear Blackboard Value");
}

EBTNodeResult::Type UMyBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey()); // a reference can't be null - access functions with point since it is not a pointer / can't modify a reference = no need for assignments so arrow is omitted
    
    return EBTNodeResult::Succeeded;
}




