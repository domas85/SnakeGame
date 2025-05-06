// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePawn.h"
#include "SnakeBodyPart.h"
#include "GameFramework/PlayerState.h"
#include "SnakePlayerState.h"


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
		/*
		if (Direction != ESnakeDirection::None)
		{
			TmpMovementMade++;
			if (TmpMovementMade > 5)
			{
				TmpMovementMade = 0;

				AteApple();
			}
		}
		*/
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
