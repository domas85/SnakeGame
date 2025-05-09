// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "SnakeWorld.generated.h"

class ASnakeGameState;
class UNode;

USTRUCT()
struct FTile
{
	GENERATED_BODY()

public:
	FVector Location;

	bool isOccupied = false;

	int gridX;

	int gridY;

	int gCost;

	int hCost;

	int fCost;

};

class  UNode
{
public:
	bool isOccupied = false;
	FVector Location;
	int gridX;
	int gridY;
	int gCost = 0;
	int hCost = 0;
	int fCost = 0;

	UNode* ParentNode = nullptr;

	UNode(bool isItOccupied, FVector loc, int x, int y)
		: isOccupied(isItOccupied), Location(loc), gridX(x), gridY(y)
	{
	}

	int fCostCalc() const { return gCost + hCost; };
	
	bool operator==(const UNode& other) const
	{
		return gridX == other.gridX && gridY == other.gridY;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSnakeMapChange);

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

	UPROPERTY()
	TArray<AActor*> Apples;

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	ASnakeGameState* GameState;

	TArray<TArray<UNode*>> GridLevel;

	TArray<FString> Levels;

	UPROPERTY()
	int32 LoadedLevel = 0;

	int gridSizeX = 0;

	int gridSizeY = 0;

	UFUNCTION()
	void CleanUpMap();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetGridLocationByIndex(int x, int y);

	UFUNCTION(BlueprintCallable)
	void SpawnApple(AActor* OldApple);

	UNode* FindTileBasedOnLocation(FVector Location);

	UNode* FindClosestAppleNode(FVector PlayerLocation);

	UFUNCTION(BlueprintCallable)
	void UpdateMap();

	UPROPERTY(BlueprintAssignable)
	FOnSnakeMapChange MapChanged;

	TArray<UNode*> GetNeighbours(UNode node);

	int MaxGridSize() { return gridSizeX * gridSizeY; };
};
