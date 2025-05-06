// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameState.h"

void ASnakeGameState::SetGameType(ESnakeGameType InGameType)
{
	if (GameType == InGameType)
	{
		return;
	}

	GameType = InGameType;
}

void ASnakeGameState::SetNextLevel()
{
	currentLevel += 1;

	if (currentLevel >= 2)
	{
		currentLevel -= 1;
	}
}