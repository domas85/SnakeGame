// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStateMachineSubsystem.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	Init,
	Menu,
	Play,
	Pause,
	GameOver,
	Quit,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameStateChanged, EGameState, OldGameState, EGameState, NewGameState);

/**
 * 
 */
UCLASS()
class SNAKEGAME_API UGameStateMachineSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	EGameState GameState = EGameState::Init;

public:
	UPROPERTY(BlueprintAssignable, Category = "GameState")
	FOnGameStateChanged OnGameStateChanged;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EGameState GetGameState() const { return GameState; }

	UFUNCTION(BlueprintCallable)
	void ChangeState(EGameState InNewState);

};
