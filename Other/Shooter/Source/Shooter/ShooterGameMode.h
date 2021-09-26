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
	const float FLOOR_WIDTH = 10000.0f;
	const float FLOOR_HEIGHT = 10000.0f;
	const float MIN_DISTANCE_BETWEEN_SPHERES = 80.0f;
	const float MIN_SPHERE_RADIUS = 30.0f;
	const float MAX_SPHERE_RADIUS = 50.0f;
	const float INNER_SPAWN_RADIUS = 1500.0f;
	const float MIN_OUTER_SPAWN_RADIUS = 2000.0f;
	const float MAX_OUTER_SPAWN_RADIUS = 3000.0f;
	const int MAX_NUM_INITIAL_SPHERES = 15;
	const int MAX_NUM_SPAWN_SPHERES = 10;

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

private:
	AActor* MainCharacter = nullptr;
	AActor* MapFloor = nullptr;

	TArray<TArray<int>> SpawnGrid;
	FVector2D SpawnGridCellSize;

	int Score = 0;
	int WaveNumber = 0;
};
