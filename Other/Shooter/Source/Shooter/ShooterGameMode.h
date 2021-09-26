// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameMode.generated.h"

class AShooterSphere;
class UShooterWidget;

UCLASS(minimalapi)
class AShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	const float MAP_FLOOR_WIDTH = 12000.0f;
	const float MAP_FLOOR_HEIGHT = 12000.0f;
	const float MIN_DISTANCE_BETWEEN_SPHERES = 80.0f;
	const float MIN_SPHERE_RADIUS = 30.0f;
	const float MAX_SPHERE_RADIUS = 50.0f;
	const float INNER_SPAWN_RADIUS = 1500.0f;
	const float MIN_OUTER_SPAWN_RADIUS = 2000.0f;
	const float MAX_OUTER_SPAWN_RADIUS = 3000.0f;
	const int MAX_NUM_INITIAL_SPHERES = 15;
	const int MAX_NUM_SPAWN_SPHERES = 10;
	const int NUM_HIT_SPHERES_BEFORE_NEXT_WAVE = MAX_NUM_SPAWN_SPHERES;
	const float MIN_SPAWN_DISTANCE_BETWEEN_SPHERES = MIN_DISTANCE_BETWEEN_SPHERES + 2.0f * MAX_SPHERE_RADIUS;

	AShooterGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void OnSphereHit(const AShooterSphere* Sphere);

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AShooterSphere> SphereClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	TSubclassOf<UShooterWidget> WidgetClass;

	UPROPERTY()
	UShooterWidget* Widget;

private:
	void LoadWidget();

	void InitSpawnGrid();
	void SpawnNextWave(int NumSpawnSpheres, int NumSpheresWithinInnerSpawnRadius);

	AActor* FindActor(FName Tag);
	void FindSpawnGridCell(const FVector& Location, int& CellX, int& CellY) const;
	bool IsSpawnGridCellFree(int CellX, int CellY) const;
	bool IsLocationWithinSpawnRegion(const FVector& Location) const;

private:
	enum class SpawnGridCellState { Free, Taken };

	AActor* MainCharacter = nullptr;
	AActor* MapFloor = nullptr;

	TArray<TArray<SpawnGridCellState>> SpawnGrid;
	FVector2D SpawnGridCellSize;
	FVector SpawnRegionCenter;
	int NumHitSpheres = 0;

	int Score = 0;
	int WaveNumber = 0;
};
