// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameMode.h"
#include "ShooterHUD.h"
#include "ShooterCharacter.h"
#include "ShooterSphere.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/GameEngine.h" // Pavlo

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

	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		const FVector Offset(-1180.0f, 800.0f, 250.0f);
		for (int x = 0; x < 5; ++x)
		{
			for (int y = 0; y < 5; ++y)
			{
				FVector Location = Offset + FVector(x * 150.0f, y * 150.0f, 0.0f);
				World->SpawnActor<AShooterSphere>(SphereClass, Location, FRotator::ZeroRotator);
			}
		}
	}
}

void AShooterGameMode::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);
}

void AShooterGameMode::OnSphereHit(const AShooterSphere* Sphere)
{
	++Score;

	// Pavlo
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "OnSphereHit");
}