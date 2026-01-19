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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
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
private:
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
	void ShowReticleWidget(bool bShow);
};
