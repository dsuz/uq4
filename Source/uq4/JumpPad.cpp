#include "JumpPad.h"

#include "UQ4Player.h"
#include "Components/TimelineComponent.h"

AJumpPad::AJumpPad()
{
	Frame = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	Pad = CreateDefaultSubobject<UStaticMeshComponent>("Pad");
	RootComponent = Frame;
	Pad->SetupAttachment(Frame);
	BouncePadTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Bounce Timeline"));
	BouncePadTimeline->SetLooping(false);
}

void AJumpPad::BeginPlay()
{
	Super::BeginPlay();
	SetupTimeline();
}

void AJumpPad::OnStepOnPad(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& Hit)
{
	this->Player = OtherActor;
	BouncePadTimeline->PlayFromStart();
}

void AJumpPad::TimelineUpdate(float Value)
{
	Pad->SetRelativeLocation(FVector(0, 0.0f, Value));
}

void AJumpPad::TimelineFinished()
{
	auto Character = Cast<AUQ4Player>(Player);
	if (Character)
	{
		auto LaunchVelocity = FVector(0, 0, Power);
		Character->LaunchCharacter(LaunchVelocity, false, true);
	}
}

void AJumpPad::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Pad->OnComponentHit.AddDynamic(this, &AJumpPad::OnStepOnPad);
}

void AJumpPad::SetupTimeline()
{
	if (BouncePadTimelineCurve)
	{
		FOnTimelineFloat UpdateDelegate;
		FOnTimelineEvent FinishDelegate;
		UpdateDelegate.BindUFunction(this, FName("TimelineUpdate"));	// 文字列じゃなくて delegate で bind できないか？
		FinishDelegate.BindUFunction(this, FName("TimelineFinished"));
		BouncePadTimeline->AddInterpFloat(BouncePadTimelineCurve, UpdateDelegate);
		BouncePadTimeline->SetTimelineFinishedFunc(FinishDelegate);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BouncePadTimelineCurve unassigned."));
	}
}
