// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacter.h"
#include "ABCharacterStatComponent.h"
#include "CountessAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "ABCharacterWidget.h"
#include "ABAIController.h"

// Sets default values
AABCharacter::AABCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	GetCharacterMovement()->MaxWalkSpeed = 800.0f;

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 450.0f;
	SpringArm->TargetOffset = FVector(0.0f, 0.0f, 100.0f);
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/ParagonCountess/Characters/Heroes/Countess/Meshes/SM_Countess.SM_Countess"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> COUNTESS_ANIM(TEXT("/Game/TheRaid/CountessAnimationBluePrint.CountessAnimationBluePrint_C"));
	if (COUNTESS_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(COUNTESS_ANIM.Class);
	}

	SetControlMode(EControlMode::Countess);
	GetCharacterMovement()->JumpZVelocity = 800.0f;
	IsAttacking = false;

	MaxCombo = 3;
	AttackEndComboState();
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Countess"));

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/TheRaid/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	AIControllerClass = AABAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();

	ABCHECK(HPBarWidget != nullptr);
	auto CharacterWidget = Cast<UABCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
		ABLOG(Warning, TEXT("BIND SUCCESS"));
	}
	else ABLOG(Warning, TEXT("BIND FAIL"));
}

void AABCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case EControlMode::Countess:
		SpringArm->TargetArmLength = 450.0f;
		SpringArm->TargetOffset = FVector(0.0f, 0.0f, 100.0f);
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;

	case EControlMode::NPC:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	}
		/*if (ControlMode == 0)
		{
			SpringArm->TargetArmLength = 450.0f;
			SpringArm->TargetOffset = FVector(0.0f, 0.0f, 100.0f);
			SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
			SpringArm->bUsePawnControlRotation = true;
			SpringArm->bInheritPitch = true;
			SpringArm->bInheritRoll = true;
			SpringArm->bInheritYaw = true;
			SpringArm->bDoCollisionTest = true;
			bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		}
		if (NPC == 1)
		{
			bUseControllerRotationYaw = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		}*/
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CountessAnim = Cast<UCountessAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != CountessAnim);

	CountessAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);

	CountessAnim->OnNextAttackCheck.AddLambda([this]()->void {
		ABLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			AttackStartComboState();
			CountessAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	CountessAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);

	CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
		ABLOG(Warning, TEXT("OnHPIsZero"));
		CountessAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	});
}

float AABCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent,
		EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	if (FinalDamage > 0.0f)
	{
		//CountessAnim->SetDeadAnim();
		//SetActorEnableCollision(false);
		
	}
	CharacterStat->SetDamage(FinalDamage);
	return FinalDamage;
}

void AABCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		SetControlMode(EControlMode::Countess);
		GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	}
	else
	{
		SetControlMode(EControlMode::NPC);
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}

// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Genesis"), EInputEvent::IE_Pressed, this, &AABCharacter::Genesis);
	PlayerInputComponent->BindAction(TEXT("Healing"), EInputEvent::IE_Pressed, this, &AABCharacter::Healing);
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::Updown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);
	
}

void AABCharacter::Updown(float NewAxisValue)
{
	if (CountessAnim->IsGenesis == true || CountessAnim->IsHealing == true)
	{
		return;
	}
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}

void AABCharacter::LeftRight(float NewAxisValue)
{
	if (CountessAnim->IsGenesis == true || CountessAnim->IsHealing == true)
	{
		return;
	}
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}

void AABCharacter::LookUp(float NewAxisValue)
{
	if (CountessAnim->IsGenesis == true || CountessAnim->IsHealing == true)
	{
		return;
	}
	AddControllerYawInput(NewAxisValue);
}

void AABCharacter::Turn(float NewAxisValue)
{
	if (CountessAnim->IsGenesis == true || CountessAnim->IsHealing == true)
	{
		return;
	}
	AddControllerYawInput(NewAxisValue);
}

void AABCharacter::Attack()
{
	if (IsAttacking) 
	{
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();
		CountessAnim->PlayAttackMontage();
		CountessAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}

	//auto AnimInstance = Cast<UCountessAnimInstance>(GetMesh()->GetAnimInstance());
	//if (nullptr == AnimInstance) return;

	/*CountessAnim->PlayAttackMontage();
	IsAttacking = true;*/
}

void AABCharacter::Genesis()
{
	ABLOG_S(Warning);
	CountessAnim->IsGenesis = true;
}

void AABCharacter::Healing()
{
	if (CountessAnim->IsHealing) return;
	CountessAnim->IsHealing = true;
	CharacterStat->SetDamage(-100.0f);
}

void AABCharacter::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	ABCHECK(IsAttacking);
	ABCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
	OnAttackEnd.Broadcast();
}

void AABCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AABCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AABCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(50.0f),
		Params);

	if (bResult)
	{
		ABLOG_S(Warning);
		if (HitResult.Actor.IsValid())
		{
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
		}

		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
	}
}

