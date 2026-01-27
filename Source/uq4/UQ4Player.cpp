#include "UQ4Player.h"
#include "Blueprint/UserWidget.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
	
	// Camera setup
	SetupCamera();
	
	// Setup Gun
	GunMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	auto GunMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(
		TEXT("/Script/Engine.StaticMesh'/Game/UnrealQuest4_Game/Meshes/SM_WaterGun.SM_WaterGun'")).Object;
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
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

float AUQ4Player::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
                             AActor* DamageCauser)
{
	Life -= Damage;
	if (Life <= 0.f)
		Die();
	return Life;
}

void AUQ4Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bForceMoveForward)
	{
		AddMovementInput(GetActorForwardVector(), SlidingSpeed * DeltaTime, true);
	}
}

void AUQ4Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
}

void AUQ4Player::Move(const FVector2D Value)
{
	if (Value.IsNearlyZero())
	{
		return;
	}
	const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
	const FRotationMatrix RotationMatrix(YawRotation);
	AddMovementInput(RotationMatrix.GetUnitAxis(EAxis::X), Value.Y);
	AddMovementInput(RotationMatrix.GetUnitAxis(EAxis::Y), Value.X);
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

void AUQ4Player::StartSliding()
{
	if (GetCharacterMovement()->MaxWalkSpeed == SlidingSpeed) return;	// スライディングしている間はこの操作を無効にする
	// スライディングしている間は制御不能にする
	bForceMoveForward = true;
	auto OriginalWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = SlidingSpeed;
	TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController());
	//if (PlayerController)
		//DisableInput(PlayerController);	// スライディングしている間も少し制御できた方が面白いので、制御を無効にしない
	auto AnimInstance = GetMesh()->GetAnimInstance();
	
	if (SlidingAnimMontage && AnimInstance)
	{
		AnimInstance->Montage_Play(SlidingAnimMontage);
		// Anim Montage の再生が終了した時の処理はラムダ式の匿名関数を作ってバインドする
		auto MontageEndDelegate = AnimInstance->Montage_GetEndedDelegate(SlidingAnimMontage);
		MontageEndDelegate->BindWeakLambda(this, [this, PlayerController, OriginalWalkSpeed](UAnimMontage* FRepRootMotionMontage, bool bInterrupted)
		{
			bForceMoveForward = false;
			GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed;
			//EnableInput(PlayerController);
		});
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Sliding Anim Montage or Invalid Animation Settings"));
	}
}

void AUQ4Player::ShootProjectile()
{
	// 不正な状態の時はログを出してアーリーリターンする
	if (ProjectileClass.Get() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Projectile Class"));
		return;
	}
	if (State != EPlayerState::FreeRun && State != EPlayerState::Aim)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid player state"));
		return;
	}

	auto Origin = GetActorLocation() + FVector(0, 0, 20); // FreeRun の時の値
	auto Target = Origin + 10000 * GetActorForwardVector(); // FreeRun の時の値
	auto ProjectileRotation = GetActorRotation(); // FreeRun で Ray が当たらなかった時の値
	auto CameraManager = UGameplayStatics::GetPlayerController(this, 0)->PlayerCameraManager;

	if (State == EPlayerState::Aim)
	{
		Origin = CameraManager->GetCameraLocation();
		auto Direction = CameraManager->GetCameraRotation().Vector();
		Target = Origin + Direction * 10000;
	}

	FHitResult Hit;

	if (bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Origin, Target, ECollisionChannel::ECC_Visibility))
	{
		ProjectileRotation = (Hit.Location - Muzzle->GetComponentLocation()).Rotation();
	}
	else if (State == EPlayerState::Aim)
	{
		ProjectileRotation = CameraManager->GetCameraRotation();
	}	// Aim モードで Ray が当たらなかったら、カメラが向いている方向に弾を飛ばす

	FActorSpawnParameters SpawnInfo;
	GetWorld()->SpawnActor<AActor>(ProjectileClass.Get(), Muzzle->GetComponentLocation(), ProjectileRotation, SpawnInfo);
}

void AUQ4Player::Die()
{
	ActivateRagdoll();
}

void AUQ4Player::Look(const FVector2D Value)
{
	AddControllerPitchInput(Value.Y);
	AddControllerYawInput(Value.X);
	LimitAimAngle();
}

void AUQ4Player::LimitAimAngle()
{
	if (State == EPlayerState::Aim)
	{
		auto PlayerController = this->GetController();
		auto Rotation = PlayerController->GetControlRotation();
		auto ClampedPitchAngle = UKismetMathLibrary::ClampAngle(Rotation.Pitch, AimLimitMin, AimLimitMax);
		Rotation.Pitch = ClampedPitchAngle;
		PlayerController->SetControlRotation(Rotation);
	}
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

void AUQ4Player::ActivateRagdoll()
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->DisableMovement();
	}
 
	// Disable player input
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}
 
	// Detach controller (optional)
	DetachFromControllerPendingDestroy();
 
	// Set collision profile for ragdoll
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
 
	// Enable physics simulation on all bones
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->bBlendPhysics = true;
 
	// Optional: stop animation
	GetMesh()->SetAnimation(nullptr);
 
	// Detach Gun
	GunMeshComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	GunMeshComp->SetSimulatePhysics(true);
	//GetMesh()->SetCollisionProfileName(TEXT("PhysicsActor"));
}

void AUQ4Player::SetupCamera()
{
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
