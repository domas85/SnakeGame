// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SnakeGameMode.generated.h"

class ASnakePlayerState;

/**
 * 
 */
UCLASS()
class SNAKEGAME_API ASnakeGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	void AppleEaten(ASnakePlayerState* PlayerState);

	void CheckAppleCount();
};
