// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"

#include "SAdvancedRotationInputBox.h"
#include "GameFramework/MovementComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialIREmitter.h"
#include "PhysicsEngine/PhysicsSettings.h"

ATarget::ATarget()
{
	PrimaryActorTick.bCanEverTick = true;
	auto ObjectFinder = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/UnrealQuest4_Game/Meshes/SM_Target.SM_Target'"));
	if (ObjectFinder.Succeeded())
	{
		TargetMesh = ObjectFinder.Object;		
	}
	
	auto SMeshComp = GetStaticMeshComponent();
	SMeshComp->SetStaticMesh(TargetMesh);
	SMeshComp->SetGenerateOverlapEvents(true);
}

void ATarget::BeginPlay()
{
	Super::BeginPlay();
	OriginalLocation = GetActorLocation();
}

void ATarget::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	float X = XAmplitude * UKismetMathLibrary::Sin(XVelocity * Timer);
	float Y = YAmplitude * UKismetMathLibrary::Cos(YVelocity * Timer);
	float Z = ZAmplitude * UKismetMathLibrary::Sin(ZVelocity * Timer);
	SetActorLocation(FVector(X, Y, Z) + OriginalLocation);
	Timer += DeltaSeconds;
	if (Timer >= UKismetMathLibrary::GetPI() * 2)
		Timer = 0.f;
}

void ATarget::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Target is overlaped by: %s"), *OtherActor->GetName()));
	
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
