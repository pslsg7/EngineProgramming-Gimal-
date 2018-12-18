// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RandomAIState.generated.h"

/**
 * 
 */
UCLASS()
class THERAID_API URandomAIState : public UBTTaskNode
{
	GENERATED_BODY()
	
	URandomAIState();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
