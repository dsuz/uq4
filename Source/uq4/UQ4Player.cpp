// Fill out your copyright notice in the Description page of Project Settings.


#include "UQ4Player.h"

#include "Blueprint/UserWidget.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AUQ4Player::AUQ4Player()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set skeletal mesh
	auto SkeletalMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>(
		TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'")).Object;
	if (SkeletalMesh)
	{
		this->GetMesh()->SetSkeletalMesh(SkeletalMesh);
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
	// Setup Gun
	GunMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	auto GunMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/UnrealQuest4_Game/Meshes/SM_WaterGun.SM_WaterGun'")).Object;
	auto SkeletalMeshComp = GetMesh();
	GunMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (GunMesh && SkeletalMesh)
	{
		GunMeshComp->SetStaticMesh(GunMesh);
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		GunMeshComp->SetupAttachment(SkeletalMeshComp, TEXT("Weapon_R"));
		Muzzle = CreateDefaultSubobject<UArrowComponent>(TEXT("Muzzle"));
		if (Muzzle)
		{
			Muzzle->SetupAttachment(GunMeshComp);
			Muzzle->SetRelativeRotation(FRotator(0, 90, 0));
		}
	}
}

void AUQ4Player::BeginPlay()
{
	Super::BeginPlay();
}

void AUQ4Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

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
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AUQ4Player::Shoot);
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
}

void AUQ4Player::Shoot()
{
	if (!bCanShoot) return;
	bCanShoot = false;
	// モンタージュを再生する
	if (ShootAnimMontage)
		PlayAnimMontage(ShootAnimMontage);
	// 連射制限の時間が経過したらフラグを戻す
	GetWorldTimerManager().SetTimer(DelayTimerHandle,
	                                FTimerDelegate::CreateWeakLambda(this, [this]() { bCanShoot = true; }),
	                                ShootInterval, false);
}

void AUQ4Player::ShootProjectile()
{
	if (ProjectileClass.Get() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Projectile Class"));
		return;
	}
	
	if (State == EPlayerState::FreeRun)
		ShootProjectileInFreeRunMode();
	if (State == EPlayerState::Aim)
		ShootProjectileInAimMode();
}

void AUQ4Player::ShootProjectileInFreeRunMode()
{
	auto Origin = GetActorLocation() + FVector(0, 0, 20); 
	auto Target = Origin + 10000 * GetActorForwardVector();
	FHitResult Hit;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Origin, Target, ECollisionChannel::ECC_Visibility);
	// DrawDebugLine(GetWorld(), Origin, Target, FColor::Red, false, 0.2f, 0, 1.0f);
	
	if (bHit)
	{
		auto Rotation = (Hit.Location - Muzzle->GetComponentLocation()).Rotation();
		FActorSpawnParameters SpawnInfo;
		GetWorld()->SpawnActor<AActor>(ProjectileClass.Get(), Muzzle->GetComponentLocation(), Rotation, SpawnInfo);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ray no hit"));
	}
}

void AUQ4Player::ShootProjectileInAimMode()
{
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
	SwitchAiming(EPlayerState::Aim);
}

void AUQ4Player::StopAiming()
{
	SwitchAiming(EPlayerState::FreeRun);
}

void AUQ4Player::SwitchAiming(EPlayerState NewState)
{
	State = NewState;
	
	switch (NewState)
	{
	case EPlayerState::Aim:
		ThirdPersonCamera->Deactivate();
		OverTheShoulderCamera->Activate();
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		ShowReticleWidget(true);
		break;
	case EPlayerState::FreeRun:
		ThirdPersonCamera->Activate();
		OverTheShoulderCamera->Deactivate();
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		ShowReticleWidget(false);
		break;
	}
}

void AUQ4Player::ShowReticleWidget(bool bShow)
{
	if (ReticleWidgetInstance)
	{
		ReticleWidgetInstance->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	} // 既にインスタンスがある場合は単純に表示/非表示を切り替える
	else if (bShow)
	{
		if (ReticleWidgetClass)
		{
			ReticleWidgetInstance = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(),
			                                                  ReticleWidgetClass);
		}
		if (ReticleWidgetInstance && !ReticleWidgetInstance->IsInViewport())
		{
			ReticleWidgetInstance->AddToViewport();
		}
	} // インスタンス化してない状態で呼ばれた場合は、それをする
}
