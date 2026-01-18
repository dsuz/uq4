// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/Character.h"
#include "UQ4Player.generated.h"

UCLASS()
class UQ4_API AUQ4Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUQ4Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
private:
	void MoveFowardBackward(float AxisValue);
	void MoveRightLeft(float AxisValue);
	void LookUpDown(float AxisValue);
	void LookRightLeft(float AxisValue);
	// エイム時の視点操作を制限する
	void LimitAimAngle();
};
