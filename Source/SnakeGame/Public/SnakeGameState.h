// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SnakeGameState.generated.h"

UENUM(BlueprintType)
enum class ESnakeGameType : uint8
{
	Cooperative,
	Battle,
};

/**
 * 
 */
UCLASS()
class SNAKEGAME_API ASnakeGameState : public AGameState
{
	GENERATED_BODY()

private:
	UPROPERTY()
	ESnakeGameType GameType = ESnakeGameType::Cooperative;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE ESnakeGameType GetGameType() const { return GameType; }

	UFUNCTION(BlueprintCallable)
	void SetGameType(ESnakeGameType InGameType);


};
