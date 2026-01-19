// Fill out your copyright notice in the Description page of Project Settings.


#include "UQ4Player.h"

#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AUQ4Player::AUQ4Player()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set skeletal mesh
	auto SkeleTalMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'")).Object;
	if (SkeleTalMesh)
	{
		this->GetMesh()->SetSkeletalMesh(SkeleTalMesh);
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}
	// Add SpringArm Component and Camera for third person view
	ThirdPersonCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonCameraBoom"));
	ThirdPersonCameraBoom->SetupAttachment(GetCapsuleComponent());
	ThirdPersonCameraBoom->bUsePawnControlRotation = true;
	ThirdPersonCameraBoom->TargetArmLength = 400.f;
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(ThirdPersonCameraBoom, USpringArmComponent::SocketName);
	// Add SpringArm Component and Camera for over-the-shoulder view
	OverTheShoulderCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("OverTheShoulderCameraBoom"));
	OverTheShoulderCameraBoom->SetupAttachment(GetCapsuleComponent());
	OverTheShoulderCameraBoom->bUsePawnControlRotation = true;
	OverTheShoulderCameraBoom->TargetArmLength = 100.f;
	OverTheShoulderCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OverTheShoulderCamera"));
	OverTheShoulderCamera->SetupAttachment(OverTheShoulderCameraBoom, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void AUQ4Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUQ4Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUQ4Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AUQ4Player::MoveFowardBackward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AUQ4Player::MoveRightLeft);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &AUQ4Player::LookRightLeft);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &AUQ4Player::LookUpDown);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AUQ4Player::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AUQ4Player::StopJumping);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AUQ4Player::StartAiming);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AUQ4Player::StopAiming);
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
}

void AUQ4Player::MoveFowardBackward(float AxisValue)
{
	const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, AxisValue);
}

void AUQ4Player::MoveRightLeft(float AxisValue)
{
	const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, AxisValue);
}

void AUQ4Player::LookUpDown(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
	LimitAimAngle();
}

void AUQ4Player::LookRightLeft(float AxisValue)
{
	AddControllerYawInput(AxisValue);
	LimitAimAngle();
}

void AUQ4Player::LimitAimAngle()
{
}

void AUQ4Player::StartAiming()
{
	if (State == EPlayerState::FreeRun)
	{
		State = EPlayerState::Aim;
		ThirdPersonCamera->Deactivate();
		OverTheShoulderCamera->Activate();
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		ShowReticleWidget(true);		
	}
}

void AUQ4Player::StopAiming()
{
	if (State == EPlayerState::Aim)
	{
		State = EPlayerState::FreeRun;
		ThirdPersonCamera->Activate();
		OverTheShoulderCamera->Deactivate();
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		ShowReticleWidget(false);		
	}
}

void AUQ4Player::ShowReticleWidget(bool bShow)
{
	if (ReticleWidgetInstance)
	{
		ReticleWidgetInstance->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	else if (bShow)
	{
		if (ReticleWidgetClass)
		{
			ReticleWidgetInstance = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), ReticleWidgetClass);
		}
		if (ReticleWidgetInstance && !ReticleWidgetInstance->IsInViewport())
		{
			ReticleWidgetInstance->AddToViewport();
		}
	}
}
