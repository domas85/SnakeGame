// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameMode.h"
#include "SnakePlayerState.h"
#include "SnakeGameState.h"

void ASnakeGameMode::AppleEaten(ASnakePlayerState* PlayerState)
{
	if (!IsValid(PlayerState))
	{
		UE_LOG(LogTemp, Error, TEXT("ASnakeGameMode::AppleEaten - >   SnakePlayerState is not valid"));
		return;
	}

	PlayerState->IncreaseSnakeSpeed(10.0f);
	CheckAppleCount();
}

void ASnakeGameMode::CheckAppleCount()
{
	ASnakeGameState* gameState = Cast<ASnakeGameState>(GetWorld()->GetGameState());

	if (!IsValid(gameState))
	{
		UE_LOG(LogTemp, Error, TEXT("ASnakeGameMode::CheckAppleCount - >   gameState is not valid"));
		return;
	}

	ASnakePlayerState* playerState = Cast<ASnakePlayerState>(gameState->PlayerArray[0]);

	if (!IsValid(playerState))
	{
		UE_LOG(LogTemp, Error, TEXT("ASnakeGameMode::CheckAppleCount - >   playerState is not valid"));
		return;
	}

	if (playerState->GetApplesEaten() >= 10)
	{
		gameState->SetNextLevel();
	}
}