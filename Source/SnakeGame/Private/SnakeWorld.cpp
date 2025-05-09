// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeWorld.h"
#include "SnakeGameState.h"

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

	Levels.Add(TEXT("Levels/Level1.txt"));
	Levels.Add(TEXT("Levels/Level2.txt"));
}

void ASnakeWorld::OnConstruction(const FTransform& Transform)
{
	UE_LOG(LogTemp, Log, TEXT("OnConstruction called"));

	CleanUpMap();

	// Load the level data from a file
	TArray<FString> Lines;
	FString FilePath = FPaths::ProjectDir() + Levels[0];

	if (FFileHelper::LoadFileToStringArray(Lines, *FilePath))
	{
		int y = 0;
		for (const FString& Line : Lines)
		{
			for (int x = 0; x < Line.Len(); x++)
			{
				FTransform Offset = FTransform(FRotator::ZeroRotator, FVector(((Lines.Num() - y) * 100.0f) - 100.0f, x * 100.0f, 0.0f));

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

void ASnakeWorld::CleanUpMap()
{
	InstancedFloors->ClearInstances();

	for (auto& Actor : SpawnActors)
	{
		if (IsValid(Actor))
		{
			Actor->Destroy();
			UE_LOG(LogTemp, Log, TEXT("Destroying Actor"));
		}
	}
	SpawnActors.Empty();
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
	FString FilePath = FPaths::ProjectDir() + Levels[0];

	LoadedLevel = 0;

	if (FFileHelper::LoadFileToStringArray(Lines, *FilePath))
	{
		GridLevel.SetNum(Lines.Num());
		GridLevel.Reserve(Lines.Num());
		gridSizeX = Lines.Num() - 1;
		gridSizeY = Lines[0].Len();

		int y = 0;
		for (const FString& Line : Lines)
		{
			GridLevel[(Lines.Num() - y) - 1].SetNum(Line.Len());
			GridLevel[(Lines.Num() - y) - 1].Reserve(Line.Len());

			for (int x = 0; x < Line.Len(); x++)
			{
				FTransform Offset = FTransform(FRotator::ZeroRotator, FVector(((Lines.Num() - y) * 100.0f) - 100.0f, x * 100.0f, 0.0f));

				if (Line[x] == '#')
				{
					FTile* Tile = new FTile();
					Tile->Location = Offset.GetLocation();
					Tile->isOccupied = true;
					//GridLevel[(Lines.Num() - y) - 1][x] = Tile;
					GridLevel[(Lines.Num() - y) - 1][x] = new UNode(true, Offset.GetLocation(), (Lines.Num() - y) - 1, x);
				}
				else
				{
					FTile* Tile = new FTile();
					Tile->Location = Offset.GetLocation();
					Tile->isOccupied = false;
					//GridLevel[(Lines.Num() - y) - 1][x] = Tile;
					GridLevel[(Lines.Num() - y) - 1][x] = new UNode(false, Offset.GetLocation(), (Lines.Num() - y) - 1, x);
				}
			}
			y++;
		}
	}
	//Algo::Reverse(GridLevel);

	//int x = 0;
	//int y = 0;
	//FindTileBasedOnLocation(FVector(900.0f, 2200.0f, 0.0f), x, y);

	//UE_LOG(LogTemp, Log, TEXT("Find Grid Level: %d %d"), x, y);

	//if (x <= GridLevel.Num() && x >= 0 && y <= GridLevel[0].Num() - 1 && y >= 0)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Grid Level from that index: %f %f"), GridLevel[x][y]->Location.X, GridLevel[x][y]->Location.Y);
	//	if (GridLevel[x][y]->isOccupied == true)
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("Is that tile Occupied: %s"), TEXT("true"));
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("Is that tile Occupied: %s"), TEXT("false"));
	//	}
	//}

	UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("UPersistentGameDataSubsystem::SaveGameData: Invalid world"));
	}

	GameState = Cast<ASnakeGameState>(World->GetGameState());

	if (!IsValid(GameState))
	{
		UE_LOG(LogTemp, Warning, TEXT("UPersistentGameDataSubsystem::SaveGameData: Invalid game state"));
	}
}

void ASnakeWorld::UpdateMap()
{

	if (!IsValid(GameState))
	{
		UE_LOG(LogTemp, Warning, TEXT("UPersistentGameDataSubsystem::SaveGameData: Invalid game state"));
		return;
	}
	if (LoadedLevel == GameState->GetCurrentLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("Stopping map update.. Level is the same"));
		return;
	}

	CleanUpMap();

	MapChanged.Broadcast();

	// Load the level data from a file
	TArray<FString> Lines;
	FString FilePath = FPaths::ProjectDir() + Levels[GameState->GetCurrentLevel()];
	LoadedLevel = GameState->GetCurrentLevel();

	if (FFileHelper::LoadFileToStringArray(Lines, *FilePath))
	{
		int y = 0;
		for (const FString& Line : Lines)
		{
			for (int x = 0; x < Line.Len(); x++)
			{
				FTransform Offset = FTransform(FRotator::ZeroRotator, FVector(((Lines.Num() - y) * 100.0f) - 100.0f, x * 100.0f, 0.0f));

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

					GridLevel[(Lines.Num() - y) - 1][x]->isOccupied = true;
				}
				else
				{
					InstancedFloors->AddInstance(Offset);

					GridLevel[(Lines.Num() - y) - 1][x]->isOccupied = false;
				}
			}
			y++;
		}
	}

	int applesInLevel = Apples.Num() - 1;

	for (int a = 0; a <= applesInLevel; a++)
	{
		SpawnApple(Apples[a]);
	}
}

void ASnakeWorld::SpawnApple(AActor* OldApple)
{
	UpdateMap();

	if (IsValid(OldApple))
	{
		Apples.Remove(OldApple);
		OldApple->Destroy();
	}

	while (true)
	{
		int xMaxIndex = GridLevel[0].Num();
		int yMaxIndex = GridLevel.Num();

		int x = FMath::RandRange(0, xMaxIndex - 1);
		int y = FMath::RandRange(0, yMaxIndex - 1);
		if (GridLevel[y][x]->isOccupied == false)
		{
			FTransform Offset = FTransform(FRotator::ZeroRotator, GridLevel[y][x]->Location + FVector(0.0f, 0.0f, 50.0f));
			AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(Apple, Offset, FActorSpawnParameters());
			if (IsValid(spawnedActor))
			{
				spawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				Apples.Add(spawnedActor);
			}
			break;
		}
	}
}

TArray<UNode*> ASnakeWorld::GetNeighbours(UNode node)
{
	TArray<UNode*> neighbours = TArray<UNode*>();


	neighbours.Add(GridLevel[node.gridX+1][node.gridY]);
	neighbours.Add(GridLevel[node.gridX-1][node.gridY]);
	neighbours.Add(GridLevel[node.gridX][node.gridY+1]);
	neighbours.Add(GridLevel[node.gridX][node.gridY-1]);

	//for (int x = -1; x <= 1; x++)
	//{
	//	for (int y = -1; y <= 1; y++)
	//	{
	//		if (x == 0 && y == 0) continue;

	//		int checkX = node.gridX + x;
	//		int checkY = node.gridY + y;

	//		if (checkX >= 0 && checkX < gridSizeX && checkY >= 0 && checkY < gridSizeY)
	//		{
	//			neighbours.Add(GridLevel[checkX][checkY]);
	//		}
	//	}
	//}
	return neighbours;
}

UNode* ASnakeWorld::FindTileBasedOnLocation(FVector Location)
{
	FVector LocalLocation = Location - GetActorLocation();
	int y = FMath::FloorToInt(LocalLocation.Y / 100.0f);
	int x = FMath::FloorToInt(LocalLocation.X / 100.0f);

	return GridLevel[x][y];
}

UNode* ASnakeWorld::FindClosestAppleNode(FVector PlayerLocation)
{
	float closestDistance = 999999.0f;
	UNode* closestApple = nullptr;
	for (int i = 0; i < Apples.Num(); i++)
	{
		FVector appleLocation = Apples[i]->GetActorLocation();
		float distance = FVector::Dist(PlayerLocation, appleLocation);
		if (distance < closestDistance)
		{
			closestDistance = distance;
			closestApple = FindTileBasedOnLocation(appleLocation);
		}
	}
	return closestApple;
}

// Called every frame
void ASnakeWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

