// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateMachineSubsystem.h"

void UGameStateMachineSubsystem::ChangeState(EGameState InNewState)
{
	if (GameState == InNewState)
	{
		return;
	}

	EGameState OldGameState = GameState;

	GameState = InNewState;
	
	OnGameStateChanged.Broadcast(OldGameState, GameState);
}