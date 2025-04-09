// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeWorld.h"

// Sets default values
ASnakeWorld::ASnakeWorld()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SceneComponent1 = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent1"));

	RootComponent = SceneComponent1;

	InstancedFloors = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedFloors"));

	InstancedFloors->SetupAttachment(RootComponent);

	InstancedWalls2 = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedWalls2"));

	InstancedWalls2->SetupAttachment(RootComponent);
}

void ASnakeWorld::OnConstruction(const FTransform& Transform)
{
	UE_LOG(LogTemp, Log, TEXT("OnConstruction called"));
	
	
	InstancedWalls2->ClearInstances();
	InstancedFloors->ClearInstances();

	TArray<FString> Lines;
	FString FilePath = FPaths::ProjectDir() + TEXT("Levels/Level1.txt");

	if (FFileHelper::LoadFileToStringArray(Lines, *FilePath))
	{
		int y = 0;
		for (const FString& Line : Lines)
		{
			for (int x = 0; x < Line.Len(); x++)
			{
				FTransform Offset = FTransform(FRotator::ZeroRotator, FVector((Lines.Num() - y) * 100.0f, x * 100.0f, 0.0f));


				if (Line[x] == '#')
				{
					InstancedWalls2->AddInstance(Offset);
				}
				else
				{
					InstancedFloors->AddInstance(Offset);
				}
			}
			y++;
		}
	}
}

// Called when the game starts or when spawned
void ASnakeWorld::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASnakeWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

