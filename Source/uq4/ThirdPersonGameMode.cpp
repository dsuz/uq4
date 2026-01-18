// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonGameMode.h"
#include "UQ4Player.h"

AThirdPersonGameMode::AThirdPersonGameMode()
{
	DefaultPawnClass = AUQ4Player::StaticClass();
}
