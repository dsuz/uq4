#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Gate.generated.h"

UCLASS()
class UQ4_API AGate : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AGate();
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> GoalTrigger;

protected:
	virtual void PostInitializeComponents() override;

public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                    bool bFromSweep, const FHitResult& SweepResult);
};
