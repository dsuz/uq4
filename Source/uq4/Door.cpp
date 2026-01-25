#include "Door.h"

ADoor::ADoor()
{
	DoorInside = CreateDefaultSubobject<UStaticMeshComponent>("DoorInside");
	DoorInside->SetupAttachment(RootComponent);
	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));
	DoorTimeline->SetLooping(false);
}

void ADoor::Open()
{
	DoorTimeline->PlayFromStart();
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	SetupTimeline();
}

void ADoor::SetupTimeline()
{
	if (DoorTimeline)
	{
		FOnTimelineFloat UpdateDelegate;
		FOnTimelineEvent FinishDelegate;
		UpdateDelegate.BindUFunction(this, FName("TimelineUpdate"));
		FinishDelegate.BindUFunction(this, FName("TimelineFinished"));
		DoorTimeline->AddInterpFloat(DoorTimelineCurve, UpdateDelegate);
		DoorTimeline->SetTimelineFinishedFunc(FinishDelegate);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BouncePadTimelineCurve unassigned."));
	}
}

void ADoor::Close()
{
	DoorTimeline->ReverseFromEnd();
}

void ADoor::TimelineUpdate(float Value)
{
	DoorInside->SetRelativeRotation(FRotator(0.0f, Value * 90, 0.0f));
}

void ADoor::TimelineFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Door timeline finished."));
}
