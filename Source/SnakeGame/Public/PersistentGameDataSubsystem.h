// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SnakeGameState.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PersistentGameDataSubsystem.generated.h"

USTRUCT()
struct FStoredGameData
{
	GENERATED_BODY()

	UPROPERTY()
	ESnakeGameType GameType;

};

/**
 * 
 */
UCLASS()
class SNAKEGAME_API UPersistentGameDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	int NumPlayers = 1;

	UPROPERTY()
	FStoredGameData StoredGameData;
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int GetNumPlayers() const { return NumPlayers; }

	UFUNCTION(BlueprintCallable)
	void SetNumPlayers(int InNumPlayers);

	UFUNCTION(BlueprintCallable)
	void SaveGameData();

	UFUNCTION(BlueprintCallable)
	void LoadGameData();
};
