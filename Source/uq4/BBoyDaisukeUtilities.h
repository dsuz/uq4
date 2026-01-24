#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BBoyDaisukeUtilities.generated.h"

UCLASS()
class UQ4_API UBBoyDaisukeUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// float を与えて、時間にフォーマットされた FText を返す。
	// フォーマットは MM:SS.sss とする。（時間は返さない）
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BBoyDaisukeUtilities|Time")
	static FText FloatToTime(float Seconds);
};
