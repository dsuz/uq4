#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UQ4Player.generated.h"

UCLASS()
class UQ4_API AUQ4Player : public ACharacter
{
	GENERATED_BODY()

public:
	AUQ4Player();

protected:
	virtual void BeginPlay() override;
	// Input Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	// Move Character function
	UFUNCTION(BlueprintCallable)
	void Move(const FVector2D Value);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TObjectPtr<USpringArmComponent> ThirdPersonCameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TObjectPtr<UCameraComponent> ThirdPersonCamera;

public:
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
		AActor* DamageCauser) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TObjectPtr<USpringArmComponent> OverTheShoulderCameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TObjectPtr<UCameraComponent> OverTheShoulderCamera;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EPlayerState> State = FreeRun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimLimitMin = -45;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimLimitMax = 45;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=UI)
	TSubclassOf<UUserWidget> ReticleWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> ReticleWidgetInstance;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> ShootAnimMontage;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> SlidingAnimMontage;
	UPROPERTY(EditAnywhere) // 連射制限
	float ShootInterval = 0.1;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// 発射のキー操作をした時に呼ぶ
	UFUNCTION(BlueprintCallable)
	void Shoot();
	// スライディングのキー操作をした時に呼ぶ
	UFUNCTION(BlueprintCallable)
	void StartSliding();
	// 発射のアニメーション通知を受けて呼ばれる
	UFUNCTION(BlueprintCallable)
	void ShootProjectile();
	UFUNCTION(BlueprintCallable)
	void Die();
	// 三人称視点→エイム
	UFUNCTION(BlueprintCallable)
	void StartAiming();
	// エイム→三人称視点
	UFUNCTION(BlueprintCallable)
	void StopAiming();

private:
	FTimerHandle DelayTimerHandle;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> GunMeshComp;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UArrowComponent> Muzzle;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditAnywhere)
	float MaxLife = 100;
	UPROPERTY()
	float Life = MaxLife;
	UPROPERTY(EditAnywhere)
	float SlidingSpeed = 100;
	bool bCanShoot = true;
	bool bForceMoveForward = false;
	UFUNCTION(BlueprintCallable)
	void Look(const FVector2D Value);
	// エイム時の視点操作を制限する
	void LimitAimAngle();
	// エイムを切り替える
	void SwitchAiming(EPlayerState NewState);
	// 照準の表示/非表示を切り替える
	void ShowReticleWidget(bool bShow);
	void ActivateRagdoll();
};
