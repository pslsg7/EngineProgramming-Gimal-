// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TheRaid.h"
#include "GameFramework/Character.h"
#include "RaidBossGrux.generated.h"

UCLASS()
class THERAID_API ARaidBossGrux : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARaidBossGrux();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent*Camera;

private:


};
