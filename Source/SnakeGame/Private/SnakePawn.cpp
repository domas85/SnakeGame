// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePawn.h"
#include "SnakeBodyPart.h"
#include "GameFramework/PlayerState.h"
#include "SnakePlayerState.h"
#include "SnakeWorld.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/BinaryHeap.h"

// Sets default values
ASnakePawn::ASnakePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	RootComponent = SceneComponent;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));

	CollisionComponent->SetupAttachment(RootComponent);
}

void ASnakePawn::PossessedBy(AController* InController)
{
	if (InController)
	{
		SetActorRotation(InController->GetControlRotation());
	}

	SnakePlayerState = InController->GetPlayerState<ASnakePlayerState>();

	if (!IsValid(SnakePlayerState))
	{
		return;
	}
}

// Called when the game starts or when spawned
void ASnakePawn::BeginPlay()
{
	Super::BeginPlay();

	SnakeWorld = Cast<ASnakeWorld>(UGameplayStatics::GetActorOfClass(GetWorld(), ASnakeWorld::StaticClass()));

	if (!IsValid(SnakeWorld))
	{
		UE_LOG(LogTemp, Warning, TEXT("SnakeWorld is not valid"));
		return;
	}

}

// Called every frame
void ASnakePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(SnakePlayerState))
	{
		return;
	}

	UpdateFalling(DeltaTime);

	UpdateMoving(DeltaTime);
}

// Called to bind functionality to input
void ASnakePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASnakePawn::UpdateFalling(float DeltaTime)
{
	FVector Position = GetActorLocation();

	// Update gravity

	VelocityY -= 10.0f * DeltaTime;

	Position.Z += VelocityY;

	if (Position.Z <= 5.0f)
	{
		Position.Z = -Position.Z;

		VelocityY = -VelocityY * 0.5f;

		if (FMath::Abs(VelocityY) < 0.1f)
		{
			VelocityY = 0.0f;

			Position.Z = 5.0f;
		}
	}

	SetActorLocation(Position);
}

void ASnakePawn::MoveSnake(float Distance)
{
	FVector Position = GetActorLocation();

	switch (Direction)
	{
	case ESnakeDirection::Up:
		Position.X += Distance;
		break;
	case ESnakeDirection::Right:
		Position.Y += Distance;
		break;
	case ESnakeDirection::Down:
		Position.X -= Distance;
		break;
	case ESnakeDirection::Left:
		Position.Y -= Distance;
		break;
	}

	SetActorLocation(Position);

	MovedTileDistance += Distance;
}


void ASnakePawn::UpdateMoving(float DeltaTime)
{
	float Speed = SnakePlayerState->GetSnakeSpeed();

	float TotalMoveDistance = Speed * DeltaTime;

	float MoveDistance = TotalMoveDistance;

	while (MovedTileDistance + MoveDistance >= TileSize)
	{
		MoveDistance = TileSize - MovedTileDistance;

		MoveSnake(MoveDistance);

		UpdateDirection();

		TotalMoveDistance -= MoveDistance;

		MoveDistance = TotalMoveDistance;

		MovedTileDistance -= TileSize;

		if (IsValid(ChildBodyPart))
		{
			ChildBodyPart->SetNextPosition(GetActorLocation());
		}
	}

	if (MoveDistance > 0.0f)
	{
		MoveSnake(MoveDistance);
	}
}

void ASnakePawn::UpdateDirection()
{
	if (DirectionsQueue.IsEmpty())
	{
		return;
	}

	Direction = DirectionsQueue[0];

	DirectionsQueue.RemoveAt(0);

	switch (Direction)
	{
	case ESnakeDirection::Up:

		CollisionComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		break;
	case ESnakeDirection::Right:

		CollisionComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		break;
	case ESnakeDirection::Down:

		CollisionComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
		break;
	case ESnakeDirection::Left:

		CollisionComponent->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
		break;
	}
}

void ASnakePawn::SetSnakeMoveDirrection(ESnakeDirection InDirection)
{
	DirectionsQueue.Add(InDirection);

	//Direction = InDirection;
}

void ASnakePawn::ClearMoveQueue()
{
	MovedTileDistance = 0.0f;
	DirectionsQueue.Empty();
}

void ASnakePawn::AteApple()
{
	UE_LOG(LogTemp, Log, TEXT("Ate Apple!"));

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	ASnakeBodyPart* BodyPart = GetWorld()->SpawnActor<ASnakeBodyPart>(BodyPartClass, GetActorLocation(), GetActorRotation(), SpawnParams);

	//BodyPart->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	if (IsValid(ChildBodyPart))
	{
		ChildBodyPart->AddChildBodyPart(BodyPart);
	}
	else
	{
		ChildBodyPart = BodyPart;
	}

	SnakePlayerState->AddApple();
}

void ASnakePawn::AStarDirection(ASnakeWorld* InGrid)
{
	if (!IsValid(InGrid))
	{
		return;
	}
	FVector PlayerLocation = GetActorLocation();

	UNode* startNode = InGrid->FindTileBasedOnLocation(PlayerLocation);
	UNode targetNode = *InGrid->FindClosestAppleNode(PlayerLocation);

	//if (currentSnakeNode != nullptr && currentSnakeNode->gridX == startNode->gridX && currentSnakeNode->gridY == startNode->gridY)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Already on the same node"));
	//	return;
	//}

	//if (currentSnakeNode == nullptr || currentSnakeNode->gridX != startNode->gridX && currentSnakeNode->gridY != startNode->gridY)
	//{
	//	currentSnakeNode = startNode;
	//}

	TArray<UNode*> openSet = TArray<UNode*>();
	TSet<UNode*> closedSet = TSet<UNode*>();
	openSet.Add(startNode);

	while (openSet.Num() > 0)
	{
		UNode* currentNode = openSet[0];
		for (int i = 1; i < openSet.Num(); i++)
		{
			if (openSet[i]->fCostCalc() < currentNode->fCostCalc() || openSet[i]->fCostCalc() == currentNode->fCostCalc() && openSet[i]->hCost < currentNode->hCost)
			{
				currentNode = openSet[i];
			}
		}
		openSet.Remove(currentNode);
		closedSet.Add(currentNode);
		if (*currentNode == targetNode)
		{
			//TArray<UNode*> path = RetracePath(startNode, currentNode);
			QueueAStarPath(startNode, currentNode);
			//if (path.Num() > 1)
			//{
			//	openSet.Empty();
			//	closedSet.Empty();

			//	
			//	//return path[path.Num() - 2]->GetDirection();
			//}
			//else
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Path less then 1"));
			//	return ESnakeDirection::Up;
			//}
		}

		//TArray<UNode> neighbours = ;
		for (UNode* neighbour : InGrid->GetNeighbours(*currentNode))
		{
			if (neighbour->isOccupied || closedSet.Contains(neighbour))
			{
				continue;
			}
			float newCostToNeighbour = currentNode->gCost + GetDistance(*currentNode, *neighbour);
			if (newCostToNeighbour < neighbour->gCost || !openSet.Contains(neighbour))
			{
				neighbour->gCost = newCostToNeighbour;
				neighbour->hCost = GetDistance(*neighbour, targetNode);
				neighbour->fCost = neighbour->gCost + neighbour->hCost;
				neighbour->ParentNode = currentNode;
				if (!openSet.Contains(neighbour))
				{
					openSet.Add(neighbour);
				}
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No path found"));
}

TArray<UNode*> ASnakePawn::RetracePath(UNode* startNode, UNode* endNode)
{
	TArray<UNode*> path = TArray<UNode*>();
	UNode* current = endNode;
	while (current != startNode)
	{
		path.Add(current);
		DrawDebugLine(GetWorld(), current->Location + FVector(0.0f, 0.0f, 50.0f), current->ParentNode->Location + FVector(0.0f, 0.0f, 50.0f), FColor::Red, false, 30.0f, 0, 5.0f);
		current = current->ParentNode;
	}
	Algo::Reverse(path);

	return path;
}

void ASnakePawn::QueueAStarPath(UNode* startNode, UNode* endNode)
{
	DirectionsQueue.Empty();

	TArray<UNode*> path = TArray<UNode*>();
	UNode* current = endNode;
	while (current != startNode)
	{
		path.Add(current);
		//DirectionsQueue.Add(GetNextMoveDirection(*current->ParentNode, *current));
		DrawDebugLine(GetWorld(), current->ParentNode->Location + FVector(-800.0f, 0.0f, 50.0f), current->Location + FVector(-800.0f, 0.0f, 50.0f), FColor::Red, false, 1.0f, 0, 5.0f);
		current = current->ParentNode;
	}
	if (!path.IsEmpty())
	{
		DirectionsQueue.Add(GetNextMoveDirection(*current, *path[path.Num() - 1]));
	}
}

ESnakeDirection ASnakePawn::GetNextMoveDirection(UNode CurrentNode, UNode NextNode)
{
	if (CurrentNode.gridX < NextNode.gridX)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Up"));
		return ESnakeDirection::Up;
	}
	else if (CurrentNode.gridX > NextNode.gridX)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Down"));
		return ESnakeDirection::Down;
	}
	else if (CurrentNode.gridY < NextNode.gridY)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Right"));
		return ESnakeDirection::Right;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Left"));
		return ESnakeDirection::Left;
	}
}

int ASnakePawn::GetDistance(UNode nodeA, UNode nodeB)
{
	int dstX = FMath::Abs(nodeA.gridX - nodeB.gridX);
	int dstY = FMath::Abs(nodeA.gridY - nodeB.gridY);

	if (dstX > dstY)
	{
		return 10 * dstY + 10 * (dstX - dstY);
	}
	else
	{
		return 10 * dstX + 10 * (dstY - dstX);
	}
}

