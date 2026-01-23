// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Target.generated.h"

/**
 * 
 */
UCLASS()
class UQ4_API ATarget : public AStaticMeshActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> TargetMesh;

public:
	ATarget();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> DeathEffect;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> SFX;
	UPROPERTY(EditAnywhere)
	float XAmplitude;
	UPROPERTY(EditAnywhere)
	float YAmplitude;
	UPROPERTY(EditAnywhere)
	float ZAmplitude;
	UPROPERTY(EditAnywhere)
	float XVelocity;
	UPROPERTY(EditAnywhere)
	float YVelocity;
	UPROPERTY(EditAnywhere)
	float ZVelocity;

private:
	FVector OriginalLocation;
	float Timer = 0.f;
};
