// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameMode.h"
#include "ShooterHUD.h"
#include "ShooterCharacter.h"
#include "ShooterSphere.h"
#include "ShooterWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"
#include "Math/IntPoint.h"

AShooterGameMode::AShooterGameMode()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

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
	SpawnNextWave(MAX_NUM_INITIAL_SPHERES, MAX_NUM_SPAWN_SPHERES);
}

void AShooterGameMode::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);

	if (NumHitSpheres == NUM_HIT_SPHERES_BEFORE_NEXT_WAVE)
		SpawnNextWave(MAX_NUM_SPAWN_SPHERES, MAX_NUM_SPAWN_SPHERES);
}

void AShooterGameMode::OnSphereHit(const AShooterSphere* Sphere)
{
	const FVector SphereLocation = Sphere->GetActorLocation();

	++Score;
	Widget->SetScore(Score);

	int SpawnGridCellX, SpawnGridCellY;
	FindSpawnGridCell(SphereLocation, SpawnGridCellX, SpawnGridCellY);

	check(SpawnGrid[SpawnGridCellY][SpawnGridCellX] == SpawnGridCellState::Taken);
	SpawnGrid[SpawnGridCellY][SpawnGridCellX] = SpawnGridCellState::Free;

	if (IsLocationWithinSpawnRegion(SphereLocation))
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

	SpawnGridCellSize = FVector2D(MIN_SPAWN_DISTANCE_BETWEEN_SPHERES);

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

void AShooterGameMode::SpawnNextWave(int NumSpawnSpheres, int NumSpheresWithinInnerSpawnRadius)
{
	check(MapFloor != nullptr);
	check(MainCharacter != nullptr);
	check(NumSpheresWithinInnerSpawnRadius <= NumSpawnSpheres);
	
	const FVector CharacterLocation = MainCharacter->GetActorLocation();
	SpawnRegionCenter = CharacterLocation;

	// To avoid possibility of collision between newly-spawned sphere and the main character,
	// we mark the cell where character is currently located as already taken.
	// Once we have generated the list of spawn locations, we restore the character cell to its original state.

	int CharacterCellX, CharacterCellY;
	FindSpawnGridCell(CharacterLocation, CharacterCellX, CharacterCellY);
	const SpawnGridCellState CharacterCellState = SpawnGrid[CharacterCellY][CharacterCellX];
	SpawnGrid[CharacterCellY][CharacterCellX] = SpawnGridCellState::Taken;

	TArray<FVector> ActiveSpawnLocations;
	ActiveSpawnLocations.Reserve(NumSpawnSpheres);

	TArray<FIntPoint> ActiveSpawnCellXYs;
	ActiveSpawnCellXYs.Reserve(NumSpawnSpheres);

	for (int i = 0; i < NumSpheresWithinInnerSpawnRadius; )
	{
		const FVector SpawnLocation = SpawnRegionCenter + FVector(FMath::RandPointInCircle(INNER_SPAWN_RADIUS), 0.0f);
		check(IsLocationWithinSpawnRegion(SpawnLocation));

		int SpawnCellX, SpawnCellY;
		FindSpawnGridCell(SpawnLocation, SpawnCellX, SpawnCellY);

		if (IsSpawnGridCellFree(SpawnCellX, SpawnCellY))
		{
			ActiveSpawnLocations.Emplace(SpawnLocation);
			ActiveSpawnCellXYs.Emplace(SpawnCellX, SpawnCellY);

			SpawnGrid[SpawnCellY][SpawnCellX] = SpawnGridCellState::Taken;
			++i;
		}
	}

	const int NumSpawnSpheresWithinOuterRadius = NumSpawnSpheres - NumSpheresWithinInnerSpawnRadius;
	for (int i = 0; i < NumSpawnSpheresWithinOuterRadius; )
	{
		const FVector SpawnLocation = SpawnRegionCenter + FVector(FMath::RandPointInCircle(MIN_OUTER_SPAWN_RADIUS), 0.0f);

		int SpawnCellX, SpawnCellY;
		FindSpawnGridCell(SpawnLocation, SpawnCellX, SpawnCellY);

		if (IsSpawnGridCellFree(SpawnCellX, SpawnCellY))
		{
			ActiveSpawnLocations.Emplace(SpawnLocation);
			ActiveSpawnCellXYs.Emplace(SpawnCellX, SpawnCellY);

			SpawnGrid[SpawnCellY][SpawnCellX] = SpawnGridCellState::Taken;
			++i;
		}
	}

	check(ActiveSpawnLocations.Num() == NumSpawnSpheres);
	for (int i = 0; i < ActiveSpawnLocations.Num(); ++i)
		GetWorld()->SpawnActor<AShooterSphere>(SphereClass, ActiveSpawnLocations[i], FRotator::ZeroRotator);

	SpawnGrid[CharacterCellY][CharacterCellX] = CharacterCellState;
		
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
	const int MinCellX = FMath::Max(CellX - 1, 0);
	const int MaxCellX = SpawnGrid[0].Num() - 1;

	const int MinCellY = FMath::Max(CellY - 1, 0);
	const int MaxCellY = SpawnGrid.Num() - 1;

	const FIntPoint NeighbourXYs[] =
	{
		FIntPoint(MinCellX, MinCellY),
		FIntPoint(MinCellX, CellY),
		FIntPoint(MinCellX, MaxCellY),

		FIntPoint(CellX, MinCellY),
		FIntPoint(CellX, CellY),
		FIntPoint(CellX, MaxCellY),

		FIntPoint(MaxCellX, MinCellY),
		FIntPoint(MaxCellX, CellY),
		FIntPoint(MaxCellX, MaxCellY)
	};

	const int NumNeighbours = sizeof(NeighbourXYs) / sizeof(NeighbourXYs[0]);
	for (int i = 0; i < NumNeighbours; ++i)
	{
		if (SpawnGrid[NeighbourXYs[i].Y][NeighbourXYs[i].X] == SpawnGridCellState::Taken)
			return false;
	}

	return true;
}

bool AShooterGameMode::IsLocationWithinSpawnRegion(const FVector& Location) const
{
	return (FVector::DistSquared(Location, SpawnRegionCenter) < INNER_SPAWN_RADIUS * INNER_SPAWN_RADIUS);
}