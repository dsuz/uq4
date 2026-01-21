#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpPad.generated.h"

UCLASS()
class UQ4_API AJumpPad : public AActor
{
	GENERATED_BODY()
	
public:	
	AJumpPad();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Frame;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Pad;
	UFUNCTION()
	void OnStepOnPad(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

public:
	virtual void PostInitializeComponents() override;
};
