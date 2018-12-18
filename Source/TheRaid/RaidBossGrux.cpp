// Fill out your copyright notice in the Description page of Project Settings.

#include "RaidBossGrux.h"

// Sets default values
ARaidBossGrux::ARaidBossGrux()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetCapsuleComponent()->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	//GetMesh()->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	SpringArm->TargetArmLength = 450.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Meshes/Grux.Grux"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> GRUX_ANIM(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Grux_AnimBlueprint.Grux_AnimBlueprint_C"));
	if (GRUX_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(GRUX_ANIM.Class);
	}

}
// Called when the game starts or when spawned
void ARaidBossGrux::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARaidBossGrux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARaidBossGrux::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

