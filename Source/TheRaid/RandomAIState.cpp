// Fill out your copyright notice in the Description page of Project Settings.

#include "RandomAIState.h"
#include "BehaviorTree/BlackboardComponent.h"


URandomAIState::URandomAIState()
{

}

EBTNodeResult::Type URandomAIState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("SkillCheck"), FMath::RandBool());
	return EBTNodeResult::Succeeded;
}
