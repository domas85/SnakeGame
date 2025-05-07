// Fill out your copyright notice in the Description page of Project Settings.


#include "PersistentGameDataSubsystem.h"

void UPersistentGameDataSubsystem::SetNumPlayers(int InNumPlayers)
{
	NumPlayers = InNumPlayers;
}

void UPersistentGameDataSubsystem::SaveGameData()
{
	UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("UPersistentGameDataSubsystem::SaveGameData: Invalid world"));
		return;
	}

	ASnakeGameState* GameState = Cast<ASnakeGameState>(World->GetGameState());

	if (!IsValid(GameState))
	{
		UE_LOG(LogTemp, Warning, TEXT("UPersistentGameDataSubsystem::SaveGameData: Invalid game state"));
		return;
	}

	StoredGameData.PlayerNames.Empty();
	StoredGameData.ApplesEatenForEachPlayer.Empty();

	TArray<APlayerState*> player_array = GameState->PlayerArray;

	for (int i = 0; i < player_array.Num(); i++)
	{
		ASnakePlayerState* player_state = Cast<ASnakePlayerState>(player_array[i]);

		if (player_state == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UPersistentGameDataSubsystem::SaveGameData: player state is null"));
			return;
		}
		StoredGameData.PlayerNames.Add(player_state->PlayerName);
		StoredGameData.ApplesEatenForEachPlayer.Add(player_state->GetApplesEaten());
	}

	// Save game date that needs to persist between levels
	StoredGameData.GameType = GameState->GetGameType();
}

void UPersistentGameDataSubsystem::LoadGameData()
{
	UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("UPersistentGameDataSubsystem::LoadGameData: Invalid world"));
		return;
	}

	ASnakeGameState* GameState = Cast<ASnakeGameState>(World->GetGameState());

	if (!IsValid(GameState))
	{
		UE_LOG(LogTemp, Warning, TEXT("UPersistentGameDataSubsystem::LoadGameData: Invalid game state"));
		return;
	}

	// Load game data between levels transitions
	GameState->SetGameType(StoredGameData.GameType);
}



