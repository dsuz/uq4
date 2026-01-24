// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ThirdPersonGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UQ4_API AThirdPersonGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;
	AThirdPersonGameMode();
	UFUNCTION(BlueprintCallable)
	void StartTimer();
	UFUNCTION(BlueprintCallable)
	void StopTimer();
	UFUNCTION(BlueprintCallable)
	void ResetTimer();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetElapsedSeconds();

private:
	float ElapsedTime = 0.0f;
	bool bIsTimerRunning = false;
};
