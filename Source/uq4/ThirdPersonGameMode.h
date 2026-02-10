#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Components/StateTreeComponent.h"
#include "ThirdPersonGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoParemeterEvent);

UCLASS()
class UQ4_API AThirdPersonGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStateTreeComponent> StateTreeComponent;

public:
	virtual void Tick(float DeltaSeconds) override;
	AThirdPersonGameMode();
	UFUNCTION(BlueprintCallable)
	void StartGame();
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FNoParemeterEvent OnGameStart;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FNoParemeterEvent OnGameFinished;
	UFUNCTION(BlueprintCallable)
	void StopGame();
	UFUNCTION(BlueprintCallable)
	void ResetTimer();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetElapsedSeconds();
	UFUNCTION(BlueprintCallable)
	void SetStateTag(FGameplayTag tag);
	
private:
	float ElapsedTime = 0.0f;
	bool bIsTimerRunning = false;
};
