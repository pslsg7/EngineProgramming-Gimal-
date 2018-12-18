// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TheRaid.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class THERAID_API AABAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AABAIController();
	virtual void Possess(APawn* InPawn) override;
	//virtual void UnPossess() override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

private:
	//void OnRepeatTimer();

	//FTimerHandle RepeatTimerHandle;
	//float RepeatInterval;
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()	
	class UBlackboardData* BBAsset;
};
