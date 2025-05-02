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

	//InstancedWalls2 = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedWalls2"));

	//InstancedWalls2->SetupAttachment(RootComponent);
}

void ASnakeWorld::OnConstruction(const FTransform& Transform)
{
	UE_LOG(LogTemp, Log, TEXT("OnConstruction called"));

	// Clean up previous instances
	// 
	//InstancedWalls2->ClearInstances();
	InstancedFloors->ClearInstances();
	for (auto& Actor : SpawnActors)
	{
		if (IsValid(Actor))
		{
			Actor->Destroy();
		}
	}
	SpawnActors.Empty();

	// Load the level data from a file
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
					if (IsValid(InstancedWalls))
					{
						AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(InstancedWalls, Offset, FActorSpawnParameters());
						if (IsValid(spawnedActor))
						{
							spawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
							SpawnActors.Add(spawnedActor);
						}
					}
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


FVector ASnakeWorld::GetGridLocationByIndex(int x, int y)
{
	return GridLevel[x][y]->Location + GetActorLocation();
}

// Called when the game starts or when spawned
void ASnakeWorld::BeginPlay()
{
	Super::BeginPlay();

	TArray<FString> Lines;
	FString FilePath = FPaths::ProjectDir() + TEXT("Levels/Level1.txt");

	if (FFileHelper::LoadFileToStringArray(Lines, *FilePath))
	{
		GridLevel.SetNum(Lines.Num());
		GridLevel.Reserve(Lines.Num());

		int y = 0;
		for (const FString& Line : Lines)
		{
			GridLevel[y].SetNum(Line.Len());
			GridLevel[y].Reserve(Line.Len());

			for (int x = 0; x < Line.Len(); x++)
			{
				FTransform Offset = FTransform(FRotator::ZeroRotator, FVector((Lines.Num() - y) * 100.0f, x * 100.0f, 0.0f));

				if (Line[x] == '#')
				{
					FTile* Tile = new FTile();
					Tile->Location = Offset.GetLocation();
					Tile->isOccupied = true;
					GridLevel[y][x] = Tile;
				}
				else
				{
					FTile* Tile = new FTile();
					Tile->Location = Offset.GetLocation();
					Tile->isOccupied = false;
					GridLevel[y][x] = Tile;
				}
			}
			y++;
		}
	}

	SpawnApple();
	int x = 0;
	int y = 0;
	FindTileBasedOnLocation(FVector(1300.0f, 100.0f, 0.0f), x, y);

	UE_LOG(LogTemp, Log, TEXT("Grid Level: %d %d"), x, y);
}

void ASnakeWorld::SpawnApple()
{
	while (true)
	{
		int xMaxIndex = GridLevel[0].Num();
		int yMaxIndex = GridLevel.Num();

		int x = FMath::RandRange(0, xMaxIndex - 1);
		int y = FMath::RandRange(0, yMaxIndex - 1);
		if (GridLevel[y][x]->isOccupied == false)
		{
			FTransform Offset = FTransform(FRotator::ZeroRotator, FVector((yMaxIndex - y) * 100.0f, x * 100.0f, 50.0f));
			AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(Apple, Offset, FActorSpawnParameters());
			if (IsValid(spawnedActor))
			{
				spawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			}
			break;
		}
	}
}

void ASnakeWorld::FindTileBasedOnLocation(FVector Location, int& x, int& y)
{
	FVector LocalLocation = Location - GetActorLocation();
	x = FMath::FloorToInt(LocalLocation.Y / 100.0f);
	y = FMath::FloorToInt(LocalLocation.X / 100.0f);
}

// Called every frame
void ASnakeWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

