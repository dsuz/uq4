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
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> DeathEffect;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> SFX;
};
