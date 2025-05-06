// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SnakePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAppleEaten, int, newAppleCount);

/**
 * 
 */
UCLASS()
class SNAKEGAME_API ASnakePlayerState : public APlayerState
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snake")
	float SnakeSpeed = 500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snake")
	int ApplesEaten = 0;

	
public:
	UPROPERTY(BlueprintAssignable, Category = "SnakeState")
	FOnAppleEaten appleEaten;

	UFUNCTION()
	FORCEINLINE int GetApplesEaten() const { return ApplesEaten; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetSnakeSpeed() const { return SnakeSpeed; }

	UFUNCTION(BlueprintCallable)
	float IncreaseSnakeSpeed(float InAddSpeed);

	UFUNCTION(BlueprintCallable)
	void AddApple();

};
