// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBodyPart.h"

// Sets default values
ASnakeBodyPart::ASnakeBodyPart()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	RootComponent = SceneComponent;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));

	CollisionComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASnakeBodyPart::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASnakeBodyPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnWithNoCollisionTime += DeltaTime;

	if (SpawnWithNoCollisionTime >= 0.15f)
	{
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	if (NextPosition != FVector::ZeroVector)
	{
		FVector Position = GetActorLocation();

		// FVector Forward = (NextPosition -= Position).GetSafeNormal(); Boids like behavior
		FVector Forward = (NextPosition - Position).GetSafeNormal();

		Position += Forward * DeltaTime * 500.0f;

		SetActorLocation(Position);
	}
}

void ASnakeBodyPart::SetNextPosition(const FVector& InPosition)
{
	if (IsValid(Child))
	{
		Child->SetNextPosition(NextPosition);
	}

	NextPosition = InPosition;
}

void ASnakeBodyPart::AddChildBodyPart(ASnakeBodyPart* InBodyPart)
{
	if (IsValid(Child))
	{
		Child->AddChildBodyPart(InBodyPart);
	}
	else
	{
		Child = InBodyPart;

		Child->SetActorLocation(GetActorLocation());
	}
}

void ASnakeBodyPart::KYS()
{
	if (IsValid(Child))
	{
		Child->KYS();
	}
	Destroy();
}

