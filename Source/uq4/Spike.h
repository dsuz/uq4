// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Spike.generated.h"

UCLASS()
class UQ4_API ASpike : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	ASpike();
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> Trigger;
	virtual void PostInitializeComponents() override;
	UFUNCTION()
	void OnStepIntoSpike(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);
};
