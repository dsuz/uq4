#pragma once

UENUM(BlueprintType)
enum EPlayerState
{
	FreeRun,
	Aim,
};

UENUM(BlueprintType)
enum EGateType
{
	Start,
	CheckPoint,
	Goal,
};

UENUM(BlueprintType)
enum EGameState
{
	None,
	InGame,
	Completed,
};