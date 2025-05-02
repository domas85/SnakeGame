// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "SnakeWorld.generated.h"

USTRUCT()
struct FTile
{
	GENERATED_BODY()

public:
	FVector Location;

	bool isOccupied = false;

};


//USTRUCT()
//struct FTile2DArray {
//	GENERATED_BODY()
//public:
//
//	TArray<FTile> Ar;
//
//	FTile operator[] (int32 i) {
//		return Ar[i];
//	}
//
//	void Add(FTile rune) {
//		Ar.Add(rune);
//	}
//};


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

	UPROPERTY()
	TArray<AActor*> SpawnActors;

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY()
	//TArray<FRune2DArray> MyArray;

	TArray<TArray<FTile*>> GridLevel;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable)
	//void CleanUpMap();

};
