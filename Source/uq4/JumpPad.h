#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
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
	UPROPERTY()
	TObjectPtr<UTimelineComponent> BouncePadTimeline;
	UPROPERTY(EditAnywhere, Category=Timeline)
	TObjectPtr<UCurveFloat> BouncePadTimelineCurve;
	UPROPERTY(EditAnywhere)
	float Power = 200;
	UFUNCTION()
	void TimelineUpdate(float Value);
	UFUNCTION()
	void TimelineFinished();

public:
	virtual void PostInitializeComponents() override;

private:
	void SetupTimeline();
	TObjectPtr<AActor> Player;
};
