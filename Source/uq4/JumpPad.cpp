#include "JumpPad.h"

AJumpPad::AJumpPad()
{
	Frame = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	Pad = CreateDefaultSubobject<UStaticMeshComponent>("Pad");
	Frame->SetupAttachment(RootComponent);
	Pad->SetupAttachment(Frame);
}

void AJumpPad::BeginPlay()
{
	Super::BeginPlay();
	
}

void AJumpPad::OnStepOnPad(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Step on pad."));
}

void AJumpPad::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Pad->OnComponentHit.AddDynamic(this, &AJumpPad::OnStepOnPad);
}
