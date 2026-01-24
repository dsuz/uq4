#include "Gate.h"

#include "ThirdPersonGameMode.h"

AGate::AGate()
{
	GoalTrigger = CreateDefaultSubobject<UBoxComponent>("GoalTrigger");
	GoalTrigger->SetupAttachment(RootComponent);
}

void AGate::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GoalTrigger)
	{
		GoalTrigger->OnComponentBeginOverlap.AddDynamic(this, &AGate::OnOverlapBegin);
		UE_LOG(LogTemp, Warning, TEXT("Bound overlap event in BeginPlay"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GoalTrigger is null in BeginPlay"));
	}
}

void AGate::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto GameModeBase = GetWorld()->GetAuthGameMode();
	auto GameMode = Cast<AThirdPersonGameMode>(GameModeBase);
	if (!GameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("Game Mode is null"));
		return;
	}
	
	switch (GateType)
	{
	case EGateType::Start:
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, TEXT("Start."));
		GameMode->StartTimer();
		break;
	case EGateType::Goal:
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, TEXT("Goal."));
		GameMode->StopTimer();
		break;
	default:
		break;
	}

}
