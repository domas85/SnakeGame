// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "SnakeWorld.generated.h"

class ASnakeGameState;

USTRUCT()
struct FTile
{
	GENERATED_BODY()

public:
	FVector Location;

	bool isOccupied = false;

};

UCLASS()
class SNAKEGAME_API ASnakeWorld : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeWorld();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* SceneComponent1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UInstancedStaticMeshComponent* InstancedFloors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> InstancedWalls;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> Apple;

	UPROPERTY()
	TArray<AActor*> SpawnActors;

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	ASnakeGameState* GameState;

	TArray<TArray<FTile*>> GridLevel;	

	TArray<FString> Levels;

	UPROPERTY()
	int32 LoadedLevel = 0;

	UFUNCTION()
	void CleanUpMap();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetGridLocationByIndex(int x, int y);

	UFUNCTION(BlueprintCallable)
	void SpawnApple();

	void FindTileBasedOnLocation(FVector Location, int& x, int& y);

	UFUNCTION(BlueprintCallable)
	void UpdateMap();
};
