// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

ATarget::ATarget()
{
	auto ObjectFinder = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/UnrealQuest4_Game/Meshes/SM_Target.SM_Target'"));
	if (ObjectFinder.Succeeded())
	{
		TargetMesh = ObjectFinder.Object;		
	}
	
	auto SMeshComp = GetStaticMeshComponent();
	SMeshComp->SetStaticMesh(TargetMesh);
	SMeshComp->SetGenerateOverlapEvents(true);
}

void ATarget::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Target is overlaped by: %s"), *OtherActor->GetName()));
	
	if (OtherActor->ActorHasTag(TEXT("Bullet")))
	{
		OtherActor->Destroy();
		if (SFX)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFX.Get(), GetActorLocation(), GetActorRotation());
		}
		if (DeathEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, GetActorLocation(), FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
		}
		this->Destroy();
	}
}
