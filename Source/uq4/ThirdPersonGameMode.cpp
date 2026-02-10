#include "ThirdPersonGameMode.h"
//#include "UQ4Player.h"
//#include "Kismet/GameplayStatics.h"
#include "UObject/EnumProperty.h"

AThirdPersonGameMode::AThirdPersonGameMode()
{
	//DefaultPawnClass = AUQ4Player::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
	this->StateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));
}

void AThirdPersonGameMode::BeginPlay()
{
	Super::BeginPlay();
	StateTreeComponent->StartLogic();
	//this->SetStateTag(FName(""));
}

void AThirdPersonGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bIsTimerRunning)	
		ElapsedTime += DeltaSeconds;
}

void AThirdPersonGameMode::StartGame()
{
	bIsTimerRunning = true;
	// if (GameState == EGameState::None)
	// {
	// 	bIsTimerRunning = true;
	// 	GameState = EGameState::InGame;
	// 	OnGameStart.Broadcast();
	// }
}

void AThirdPersonGameMode::StopGame()
{
	bIsTimerRunning = false;
	
	// if (GameState == EGameState::InGame)
	// {
	// 	bIsTimerRunning = false;
	// 	GameState = EGameState::Completed;
	// 	OnGameFinished.Broadcast();
	// }
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

// EGameState AThirdPersonGameMode::GetGameState()
// {
// 	return GameState;
// }
//
// void AThirdPersonGameMode::SetGameState(EGameState NewState)
// {
// 	UEnum* EnumPtr = StaticEnum<EGameState>();
// 	auto CurrentStateName = EnumPtr->GetNameStringByValue(static_cast<int64>(this->GameState));
// 	auto NewStateName = EnumPtr->GetNameStringByValue(static_cast<int64>(NewState));
// 	auto Message = FString::Printf(TEXT("Game State Changed %s -> %s"), *CurrentStateName, *NewStateName);
// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Message);
// 	this->GameState = NewState;
// }

void AThirdPersonGameMode::SetStateTag(FGameplayTag tag)
{
	StateTreeComponent->SendStateTreeEvent(tag);
}

