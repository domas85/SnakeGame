// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "SnakeBodyPart.generated.h"

class ASnakePlayerState;

UCLASS()
class SNAKEGAME_API ASnakeBodyPart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeBodyPart();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SnakeComponents")
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SnakeComponents")
	USphereComponent* CollisionComponent;

	UPROPERTY()
	bool bNextPositionSet = false;

	UFUNCTION()
	void AddChildBodyPart(ASnakeBodyPart* InBodyPart);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	ASnakePlayerState* snakePlayerState = nullptr;

	UPROPERTY()
	float SpawnWithNoCollisionTime = 0;

	UPROPERTY()
	FVector NextPosition = FVector::ZeroVector;

	UPROPERTY()
	ASnakeBodyPart* Child = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetNextPosition(const FVector& InPosition);

	UFUNCTION(BlueprintCallable)
	void KYS();

};
