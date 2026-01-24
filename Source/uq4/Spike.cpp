#include "Spike.h"

#include "UQ4Player.h"
#include "Kismet/GameplayStatics.h"

ASpike::ASpike()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	Trigger->SetupAttachment(RootComponent);
}

void ASpike::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASpike::OnStepIntoSpike);
}

void ASpike::OnStepIntoSpike(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto Player = Cast<AUQ4Player>(OtherActor))
	{
		UGameplayStatics::ApplyDamage(Player, 9999, nullptr, this, nullptr);
	}
}
