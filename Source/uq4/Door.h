#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Door.generated.h"

UCLASS()
class UQ4_API ADoor : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	ADoor();
	UPROPERTY()
	TObjectPtr<UTimelineComponent> DoorTimeline;
	UPROPERTY(EditAnywhere, Category=Timeline)
	TObjectPtr<UCurveFloat> DoorTimelineCurve;
	UFUNCTION(BlueprintCallable)
	void Open();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void Close();
	UFUNCTION()
	void TimelineUpdate(float Value);
	UFUNCTION()
	void TimelineFinished();
private:
	// ドアの内側の扉
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> DoorInside;
	void SetupTimeline();
};
