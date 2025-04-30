// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameMode.h"
#include "SnakePlayerState.h"

void ASnakeGameMode::AppleEaten(ASnakePlayerState* PlayerState)
{
	if (!IsValid(PlayerState))
	{
		UE_LOG(LogTemp, Error, TEXT("ASnakeGameMode::AppleEaten - >   SnakePlayerState is not valid"));
		return;
	}

	PlayerState->IncreaseSnakeSpeed(10.0f);
}