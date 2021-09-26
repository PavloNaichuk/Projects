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
	const float MIN_DISTANCE_BETWEEN_SPHERES = 80.0f;
	const float MIN_SPHERE_RADIUS = 25.0f;
	const float MAX_SPHERE_RADIUS = 50.0f;
	const float SPHERE_RADIUS_PERCENTAGE_DEC = 0.05f;
	const float INNER_SPAWN_RADIUS = 1500.0f;
	const float MIN_OUTER_SPAWN_RADIUS = 2000.0f;
	const float MAX_OUTER_SPAWN_RADIUS = 3000.0f;
	const float OUTER_SPAWN_RADIUS_PERCENTAGE_INC_PER_WAVE = 0.05f;
	const int NUM_INITIAL_SPHERES = 15;
	const int NUM_SPHERES_WITHING_INNER_RADIUS = 10;
	const float NUM_SPHERE_PERCENTAGE_INC_PER_WAVE = 0.1f;
	const int NUM_HIT_SPHERES_BEFORE_NEXT_WAVE = NUM_SPHERES_WITHING_INNER_RADIUS;
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
	void SpawnNextWave();

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
	int NumSpawnedSpheres = 0;
	float OuterSpawnRadius = 0.0f;

	int Score = 0;
	int WaveNumber = 0;
};
