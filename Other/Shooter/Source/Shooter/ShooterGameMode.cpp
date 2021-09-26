// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameMode.h"
#include "ShooterHUD.h"
#include "ShooterCharacter.h"
#include "ShooterSphere.h"
#include "ShooterWidget.h"
#include "ShooterRandom.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"
#include "Math/IntPoint.h"

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

	if (NumHitSpheres == NUM_HIT_SPHERES_BEFORE_NEXT_WAVE)
		SpawnNextWave();
}

void AShooterGameMode::OnSphereHit(const AShooterSphere* Sphere)
{
	const FVector SphereLocation = Sphere->GetActorLocation();

	++Score;
	Widget->SetScore(Score);

	int SpawnGridCellX, SpawnGridCellY;
	FindSpawnGridCell(SphereLocation, SpawnGridCellX, SpawnGridCellY);
	SpawnGrid[SpawnGridCellY][SpawnGridCellX] = SpawnGridCellState::Free;

	if (FVector::DistSquared(SphereLocation, SpawnRegionCenter) < INNER_SPAWN_RADIUS * INNER_SPAWN_RADIUS)
		++NumHitSpheres;
}

void AShooterGameMode::InitSpawnGrid()
{
	// Bulding spawn (acceleration) grid to compute spawn locations for the spheres.
	// Each spawn grid cell contains a flag to indicate if there already a sphere inside it.
	// The spawn grid is used to speed up identifying locations on the map where sphere can be spawned to keep the minumim required distance.
	// Based on the algorithm "Fast Poisson Disk Sampling in Arbitrary Dimensions" by Robert Bridson
	// https://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf

	check(MapFloor != nullptr);

	FVector FloorOrigin;
	FVector FloorBoxExtent;
	MapFloor->GetActorBounds(false, FloorOrigin, FloorBoxExtent);

	SpawnGridCellSize = FVector2D(MIN_DISTANCE_BETWEEN_SPHERES / FMath::Sqrt(2.0f));

	FVector FloorBoxSize(2.0f * FloorBoxExtent);
	const int NumCellsX = FMath::CeilToInt(FloorBoxSize.X / SpawnGridCellSize.X);
	const int NumCellsY = FMath::CeilToInt(FloorBoxSize.Y / SpawnGridCellSize.Y);

	SpawnGrid.SetNum(NumCellsY);
	for (int Y = 0; Y < NumCellsY; ++Y)
		SpawnGrid[Y].Init(SpawnGridCellState::Free, NumCellsX);
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
	check(MapFloor != nullptr);
	check(MainCharacter != nullptr);

	const FVector CharacterLocation = MainCharacter->GetActorLocation();
	
	// To avoid possibility of collision between newly-spawned sphere and the main character,
	// we mark the cell where character is currently located as already taken.
	// Once we have generated the list of spawn locations, we restore the character cell to its original state.

	int CharacterCellX, CharacterCellY;
	FindSpawnGridCell(CharacterLocation, CharacterCellX, CharacterCellY);
	const SpawnGridCellState CharacterCellState = SpawnGrid[CharacterCellY][CharacterCellX];
	SpawnGrid[CharacterCellY][CharacterCellX] = SpawnGridCellState::Taken;

	TArray<FVector> ActiveSpawnLocations;
	ActiveSpawnLocations.Reserve(MAX_NUM_SPAWN_SPHERES);

	TArray<FIntPoint> ActiveSpawnCellXYs;
	ActiveSpawnCellXYs.Reserve(MAX_NUM_SPAWN_SPHERES);

	while (ActiveSpawnLocations.Num() < MAX_NUM_SPAWN_SPHERES)
	{
		while (ActiveSpawnLocations.Num() == 0)
		{
			const FVector SpawnLocation = CharacterLocation + FVector(FMath::RandPointInCircle(INNER_SPAWN_RADIUS), 0.0f);

			int SpawnCellX, SpawnCellY;
			FindSpawnGridCell(SpawnLocation, SpawnCellX, SpawnCellY);

			if (IsSpawnGridCellFree(SpawnCellX, SpawnCellY))
			{
				ActiveSpawnLocations.Emplace(SpawnLocation);
				ActiveSpawnCellXYs.Emplace(SpawnCellX, SpawnCellY);

				SpawnGrid[SpawnCellY][SpawnCellX] = SpawnGridCellState::Taken;
			}
		}

		const int SpawnParentIndex = FMath::RandRange(0, ActiveSpawnLocations.Num() - 1);
		const FVector& SpawnParentLocation = ActiveSpawnLocations[SpawnParentIndex];

		bool ManagedToSpawnChild = false;
		for (int i = 0; i < NUM_ATTEMPTS_BEFORE_SPAWN_REJECTION; ++i)
		{
			const FVector2D PointInAnnulus = ShooterRandom::PointInAnnulus(MIN_DISTANCE_BETWEEN_SPHERES, 2.0f * MIN_DISTANCE_BETWEEN_SPHERES);
			const FVector SpawnChildLocation = SpawnParentLocation + FVector(PointInAnnulus, 0.0f);

			int SpawnChildCellX, SpawnChildCellY;
			FindSpawnGridCell(SpawnChildLocation, SpawnChildCellX, SpawnChildCellY);

			if (IsSpawnGridCellFree(SpawnChildCellX, SpawnChildCellY))
			{
				ActiveSpawnLocations.Emplace(SpawnChildLocation);
				ActiveSpawnCellXYs.Emplace(SpawnChildCellX, SpawnChildCellY);

				SpawnGrid[SpawnChildCellY][SpawnChildCellX] = SpawnGridCellState::Taken;
				ManagedToSpawnChild = true;
			}
		}

		if (!ManagedToSpawnChild)
		{
			const FIntPoint SpawnParentCellXY = ActiveSpawnCellXYs[SpawnParentIndex];
			SpawnGrid[SpawnParentCellXY.Y][SpawnParentCellXY.X] = SpawnGridCellState::Free;

			ActiveSpawnLocations.RemoveAt(SpawnParentIndex);
			ActiveSpawnCellXYs.RemoveAt(SpawnParentIndex);
		}
	}

	for (int i = 0; i < ActiveSpawnLocations.Num(); ++i)
		GetWorld()->SpawnActor<AShooterSphere>(SphereClass, ActiveSpawnLocations[i], FRotator::ZeroRotator);

	SpawnGrid[CharacterCellY][CharacterCellX] = CharacterCellState;
	SpawnRegionCenter = CharacterLocation;
	
	++WaveNumber;
	Widget->SetWaveNumber(WaveNumber);
	NumHitSpheres = 0;
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

void AShooterGameMode::FindSpawnGridCell(const FVector& Location, int& CellX, int& CellY) const
{
	check(MapFloor != nullptr);

	FVector FloorOrigin;
	FVector FloorBoxExtent;
	MapFloor->GetActorBounds(false, FloorOrigin, FloorBoxExtent);

	const FVector FloorMin = FloorOrigin - FloorBoxExtent;
	const FVector FloorMax = FloorOrigin + FloorBoxExtent;

	check(FMath::IsWithin(FVector2D(Location.X, Location.Y), FVector2D(FloorMin.X, FloorMin.Y), FVector2D(FloorMax.X, FloorMax.Y)));

	const FVector Offset = Location - FloorMin;
	CellX = FMath::FloorToInt(Offset.X / SpawnGridCellSize.X);
	CellY = FMath::FloorToInt(Offset.Y / SpawnGridCellSize.Y);
}

bool AShooterGameMode::IsSpawnGridCellFree(int CellX, int CellY) const
{
	const int MaxCellX = SpawnGrid[0].Num() - 1;
	const int MaxCellY = SpawnGrid.Num() - 1;

	const FIntPoint NeighbourXYs[] =
	{
		FIntPoint(CellX, CellY),
		FIntPoint(FMath::Max(CellX - 1, 0), CellY),
		FIntPoint(FMath::Min(CellX + 1, MaxCellX), CellY),
		FIntPoint(CellX, FMath::Max(CellY - 1, 0)),
		FIntPoint(CellX, FMath::Min(CellY + 1, MaxCellY)),
		FIntPoint(FMath::Max(CellX - 1, 0), FMath::Max(CellY - 1, 0)),
		FIntPoint(FMath::Min(CellX + 1, MaxCellX), FMath::Min(CellY + 1, MaxCellY)),
		FIntPoint(FMath::Min(CellX + 1, MaxCellX), FMath::Max(CellY - 1, 0)),
		FIntPoint(FMath::Max(CellX + 1, MaxCellX), FMath::Min(CellY + 1, MaxCellY))
	};

	const int NumNeighbours = sizeof(NeighbourXYs) / sizeof(NeighbourXYs[0]);
	for (int i = 0; i < NumNeighbours; ++i)
	{
		if (SpawnGrid[NeighbourXYs[i].Y][NeighbourXYs[i].X] == SpawnGridCellState::Taken)
			return false;
	}

	return true;
}