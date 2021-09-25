// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameMode.h"
#include "ShooterHUD.h"
#include "ShooterCharacter.h"
#include "ShooterSphere.h"
#include "ShooterWidget.h"
#include "UObject/ConstructorHelpers.h"

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

	LoadWidget();
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
	const FVector Offset(-1180.0f, 800.0f, 250.0f);
	for (int x = 0; x < 2; ++x)
	{
		for (int y = 0; y < 2; ++y)
		{
			FVector Location = Offset + FVector(x * 200.0f, y * 200.0f, 0.0f);
			GetWorld()->SpawnActor<AShooterSphere>(SphereClass, Location, FRotator::ZeroRotator);
		}
	}

	++WaveNumber;
	Widget->SetWaveNumber(WaveNumber);
}