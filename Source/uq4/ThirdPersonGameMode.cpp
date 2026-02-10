#include "ThirdPersonGameMode.h"
#include "UObject/EnumProperty.h"

AThirdPersonGameMode::AThirdPersonGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	this->StateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));
}

void AThirdPersonGameMode::BeginPlay()
{
	Super::BeginPlay();
	StateTreeComponent->StartLogic();
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
}

void AThirdPersonGameMode::StopGame()
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

void AThirdPersonGameMode::SetStateTag(FGameplayTag tag)
{
	StateTreeComponent->SendStateTreeEvent(tag);
}

