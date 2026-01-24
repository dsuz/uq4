#include "Gate.h"

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
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, TEXT("Goal."));
}
