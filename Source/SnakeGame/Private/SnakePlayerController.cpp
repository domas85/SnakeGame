// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePlayerController.h"

void ASnakePlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitInput();
}

void ASnakePlayerController::InitInput()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (!IsValid(Subsystem))
		{
			UE_LOG(LogTemp, Error, TEXT("ASnakePlayerController::InitInput - >   Subsystem is not valid"));
			return;
		}

		if (!IsValid(DefaultMappingContext))
		{
			UE_LOG(LogTemp, Error, TEXT("ASnakePlayerController::InitInput - >   DefaultMappingContext is not valid"));

		}

		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ASnakePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);

	if (!IsValid(Input))
	{
		UE_LOG(LogTemp, Error, TEXT("InputComponent is not valid"));
		return;

	}
	if (!IsValid(MoveAction))
	{
		UE_LOG(LogTemp, Error, TEXT("MoveAction is not valid"));
	}

	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASnakePlayerController::Move);
}

void ASnakePlayerController::Move(const FInputActionValue& Value)
{
	ASnakePawn* MySnake = Cast<ASnakePawn>(GetPawn());

	if (!MySnake)
	{
		UE_LOG(LogTemp, Error, TEXT("MySnake is not valid"));
		return;
	}

	

}