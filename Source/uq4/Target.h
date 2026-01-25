#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "Engine/StaticMeshActor.h"
#include "Target.generated.h"

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
	UPROPERTY(EditAnywhere)
	TObjectPtr<ADoor> TargetDoor;
	virtual void Destroyed() override;

private:
	FVector OriginalLocation;
	float Timer = 0.f;
};
