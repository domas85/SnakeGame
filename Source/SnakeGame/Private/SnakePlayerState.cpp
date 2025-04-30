// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePlayerState.h"
#include "SnakeGameMode.h"
#include "Kismet/GameplayStatics.h"

float ASnakePlayerState::IncreaseSnakeSpeed(float InAddSpeed)
{
	SnakeSpeed += InAddSpeed;
	return SnakeSpeed;
}

void ASnakePlayerState::AddApple()
{
	ApplesEaten++;

	ASnakeGameMode* SnakeGameMode = Cast<ASnakeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!IsValid(SnakeGameMode))
	{
		UE_LOG(LogTemp, Error, TEXT("ASnakePlayerState::AddApple - >   SnakeGameMode is not valid"));
		return;
	}


	SnakeGameMode->AppleEaten(this);

	appleEaten.Broadcast(ApplesEaten);
}