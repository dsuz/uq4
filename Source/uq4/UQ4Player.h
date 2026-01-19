// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "UQ4Player.generated.h"

UCLASS()
class UQ4_API AUQ4Player : public ACharacter
{
	GENERATED_BODY()

public:
	AUQ4Player();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	USpringArmComponent* ThirdPersonCameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	UCameraComponent* ThirdPersonCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	USpringArmComponent* OverTheShoulderCameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	UCameraComponent* OverTheShoulderCamera;

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
	UUserWidget* ReticleWidgetInstance;
	UPROPERTY(EditAnywhere)
	UAnimMontage* ShootAnimMontage;
	UPROPERTY(EditAnywhere)
	float ShootInterval = 0.1;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// 発射のキー操作をした時に呼ぶ
	void Shoot();
	// 発射のアニメーション通知を受けて呼ばれる
	UFUNCTION(BlueprintCallable)
	void ShootProjectile();

private:
	FTimerHandle DelayTimerHandle;
	bool bCanShoot = true;
	void MoveFowardBackward(float AxisValue);
	void MoveRightLeft(float AxisValue);
	void LookUpDown(float AxisValue);
	void LookRightLeft(float AxisValue);
	// エイム時の視点操作を制限する
	void LimitAimAngle();
	// 三人称視点→エイム
	void StartAiming();
	// エイム→三人称視点
	void StopAiming();
	// 昇順の表示/非表示を切り替える
	void ShowReticleWidget(bool bShow);
};
