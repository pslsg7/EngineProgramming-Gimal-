// Fill out your copyright notice in the Description page of Project Settings.

#include "CountessAnimInstance.h"
#include "ABCharacterStatComponent.h"
#include "ABCharacter.h"
 

UCountessAnimInstance::UCountessAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
	IsGenesis = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/TheRaid/S_Countess_Skeleton_Montage.S_Countess_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	//static ConstructorHelpers::FObjectFinder<UAnimInstance> ATTACK(TEXT("))
}

void UCountessAnimInstance::NativeUpdateAnimation(float DelataSeconds)
{
	Super::NativeUpdateAnimation(DelataSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn))
	{
		return;
	}

	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UCountessAnimInstance::PlayAttackMontage()
{
	ABCHECK(!IsDead);
	Montage_Play(AttackMontage, 1.0f);	
}

void UCountessAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	ABCHECK(!IsDead);
	ABCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection),
		AttackMontage);
}

void UCountessAnimInstance::SetGenesis()
{
	ABCHECK(!IsGenesis);
	IsGenesis = true;
}

void UCountessAnimInstance::SetHealing()
{
	IsHealing = true;
}

void UCountessAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
	//ABLOG_S(Warning);
}

void UCountessAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UCountessAnimInstance::AnimNotify_GenesisEnd()
{
	IsGenesis = false;
}

void UCountessAnimInstance::AnimNotify_HealingEnd()
{
	IsHealing = false;
}

void UCountessAnimInstance::AnimNotify_ApplyDamageGenesis()
{
	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn))
	{
		return;
	}
	TArray<AActor*> Actors;
	Actors.Add(Pawn);
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 50.0f, Pawn->GetActorLocation(), 500.0f, nullptr, Actors);
}

void UCountessAnimInstance::AnimNotify_Healing()
{
}

FName UCountessAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"),Section));
}
