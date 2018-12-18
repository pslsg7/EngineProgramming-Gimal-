// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TheRaid.h"
#include "Animation/AnimInstance.h"
#include "CountessAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

/**
 * 
 */
UCLASS()
class THERAID_API UCountessAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UCountessAnimInstance();
	virtual void NativeUpdateAnimation(float DelataSeconds) override;

	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	void SetDeadAnim() { IsDead = true; }
	void SetGenesis();
	void SetHealing();

private:
	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	UFUNCTION()
	void AnimNotify_GenesisEnd();

	UFUNCTION()
	void AnimNotify_HealingEnd();

	UFUNCTION()
	void AnimNotify_ApplyDamageGenesis();

	UFUNCTION()
	void AnimNotify_Healing();

	FName GetAttackMontageSectionName(int32 Section);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn,
		Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn,
		Meta = (AllowPrivateAccess = true))
		bool IsInAir;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack,
		Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn,
		Meta = (AllowPrivateAccess = true))
		bool IsDead;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn,
	Meta = (AllowPrivateAccess = true))
		bool IsGenesis;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn,
		Meta = (AllowPrivateAccess = true))
		bool IsHealing;
	
};
