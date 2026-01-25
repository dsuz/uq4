#include "ThirdPersonGameMode.h"
//#include "UQ4Player.h"

AThirdPersonGameMode::AThirdPersonGameMode()
{
	//DefaultPawnClass = AUQ4Player::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
}

void AThirdPersonGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bIsTimerRunning)	
		ElapsedTime += DeltaSeconds;
}

void AThirdPersonGameMode::StartGame()
{
	if (GameState == EGameState::None)
	{
		bIsTimerRunning = true;
		GameState = EGameState::InGame;
		OnGameStart.Broadcast();
	}
}

void AThirdPersonGameMode::StopGame()
{
	if (GameState == EGameState::InGame)
	{
		bIsTimerRunning = false;
		GameState = EGameState::Completed;
		OnGameFinished.Broadcast();
	}
}

void AThirdPersonGameMode::ResetTimer()
{
	bIsTimerRunning = false;
	ElapsedTime = 0.0f;
}

float AThirdPersonGameMode::GetElapsedSeconds()
{
	return ElapsedTime;
}

EGameState AThirdPersonGameMode::GetGameState()
{
	return GameState;
}

