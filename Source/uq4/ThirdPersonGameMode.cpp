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

void AThirdPersonGameMode::StartTimer()
{
	bIsTimerRunning = true;
}

void AThirdPersonGameMode::StopTimer()
{
	bIsTimerRunning = false;
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

