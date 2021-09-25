// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameMode.generated.h"

class AShooterSphere;

UCLASS(minimalapi)
class AShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AShooterGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AShooterSphere> SphereClass;

	void OnSphereHit(const AShooterSphere* Sphere);

private:
	int Score = 0;
	int WaveNumber = 0;
};



