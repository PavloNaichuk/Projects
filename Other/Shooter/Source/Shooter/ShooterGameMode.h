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
	void SpawnNextWave();

private:
	int Score = 0;
	int WaveNumber = 0;
};
