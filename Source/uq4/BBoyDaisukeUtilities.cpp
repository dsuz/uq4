// Fill out your copyright notice in the Description page of Project Settings.


#include "BBoyDaisukeUtilities.h"

FText UBBoyDaisukeUtilities::FloatToTime(float Seconds)
{
	const int IMinutes = Seconds / 60;
	const int ISeconds = static_cast<int>(Seconds) % 60;
	const int IMilliseconds = floor((Seconds - ISeconds) * 1000);
	auto MillisecondsString = FString::Printf(TEXT("%03d"), IMilliseconds);
	auto SecondsString = FString::Printf(TEXT("%02d"), ISeconds);
	auto MinutesString = FString::Printf(TEXT("%02d"), IMinutes);
	auto TimeFormatString = FString::Printf(TEXT("%s:%s.%s"), *MinutesString, *SecondsString, *MillisecondsString);
	return FText::FromString(TimeFormatString);
}
