// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Definitions.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "SnakePawn.generated.h"

UCLASS()
class SNAKEGAME_API ASnakePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASnakePawn();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SnakeComponents")
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SnakeComponents")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SnakeComponents")
	ESnakeDirection Direction = ESnakeDirection::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SnakeComponents")
	float MovedTileDistance = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SnakeComponents")
	FRotator ForwardRotation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float VelocityY = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ESnakeDirection> DirectionsQueue;

	UFUNCTION()
	void UpdateDirection();

	UFUNCTION()
	void UpdateFalling(float DeltaTime);

	UFUNCTION()
	void UpdateMoving(float DeltaTime);

	UFUNCTION()
	void MoveSnake(float Distance);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetSnakeMoveDirrection(ESnakeDirection InDirection);

};
