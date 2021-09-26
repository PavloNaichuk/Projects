// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameMode.h"
#include "ShooterHUD.h"
#include "ShooterCharacter.h"
#include "ShooterSphere.h"
#include "ShooterWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"

namespace
{
	const int INVALID_INDEX = -1;
}

AShooterGameMode::AShooterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AShooterHUD::StaticClass();
}

void AShooterGameMode::BeginPlay() 
{
	Super::BeginPlay();

	MainCharacter = FindActor(FName(TEXT("MainCharacter")));
	MapFloor = FindActor(FName(TEXT("MapFloor")));

	LoadWidget();

	InitSpawnGrid();
	SpawnNextWave();
}

void AShooterGameMode::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);
}

void AShooterGameMode::OnSphereHit(const AShooterSphere* Sphere)
{
	++Score;
	Widget->SetScore(Score);

	if ((Score % 4) == 0)
		SpawnNextWave();
}

void AShooterGameMode::InitSpawnGrid()
{
	// Bulding spawn (acceleration) grid to compute spawn locations for the spheres.
	// Each spawn grid cell contains index of the sphere at that location on the map.
	// The index is set to INVALID_INDEX if there is no sphere in that cell.
	// The spawn grid is used to speed up identifying locations on the map where sphere could be spawned
	// so that we maintain the minumim required distance.
	// Based on the algorithm "Fast Poisson Disk Sampling in Arbitrary Dimensions" by Robert Bridson
	// https://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf

	check(MapFloor != nullptr);

	FVector FloorOrigin;
	FVector FloorBoxExtent;
	MapFloor->GetActorBounds(false, FloorOrigin, FloorBoxExtent);

	SpawnGridCellSize = FVector2D(MIN_DISTANCE_BETWEEN_SPHERES / FMath::Sqrt(2.0f));

	FVector FloorBoxSize(2.0f * FloorBoxExtent);
	const int numCellsX = FMath::CeilToInt(FloorBoxSize.X / SpawnGridCellSize.X);
	const int numCellsY = FMath::CeilToInt(FloorBoxSize.Y / SpawnGridCellSize.Y);

	SpawnGrid.SetNum(numCellsY);
	for (int y = 0; y < numCellsY; ++y)
		SpawnGrid[y].Init(INVALID_INDEX, numCellsX);
}

void AShooterGameMode::LoadWidget()
{
	if (Widget != nullptr)
	{
		Widget->RemoveFromViewport();
		Widget = nullptr;
	}

	if (WidgetClass != nullptr)
	{
		Widget = CreateWidget<UShooterWidget>(GetWorld(), WidgetClass);
		if (Widget != nullptr)
		{
			Widget->AddToViewport();
		}
	}

	Widget->Load();
}

void AShooterGameMode::SpawnNextWave()
{
	/*
	const FVector Offset(-1180.0f, 800.0f, 250.0f);
	for (int x = 0; x < 2; ++x)
	{
		for (int y = 0; y < 2; ++y)
		{
			FVector Location = Offset + FVector(x * 200.0f, y * 200.0f, 0.0f);
			GetWorld()->SpawnActor<AShooterSphere>(SphereClass, Location, FRotator::ZeroRotator);
		}
	}
	*/

	++WaveNumber;
	Widget->SetWaveNumber(WaveNumber);
}

AActor* AShooterGameMode::FindActor(FName Tag)
{
	for (FActorIterator Iter(GetWorld()); Iter; ++Iter)
	{
		AActor* Actor = *Iter;
		if (Actor->ActorHasTag(Tag))
			return Actor;
	}
	return nullptr;
}