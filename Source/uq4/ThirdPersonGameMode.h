#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/GameModeBase.h"
#include "ThirdPersonGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoParemeterEvent);

UCLASS()
class UQ4_API AThirdPersonGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;
	AThirdPersonGameMode();
	UFUNCTION(BlueprintCallable)
	void StartGame();
	UPROPERTY(BlueprintAssignable)
	FNoParemeterEvent OnGameStart;
	UPROPERTY(BlueprintAssignable)
	FNoParemeterEvent OnGameFinished;
	UFUNCTION(BlueprintCallable)
	void StopGame();
	UFUNCTION(BlueprintCallable)
	void ResetTimer();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetElapsedSeconds();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EGameState GetGameState();

private:
	float ElapsedTime = 0.0f;
	bool bIsTimerRunning = false;
	EGameState GameState = EGameState::None;
};
