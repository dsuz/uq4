// Fill out your copyright notice in the Description page of Project Settings.


#include "UQ4Player.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AUQ4Player::AUQ4Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
