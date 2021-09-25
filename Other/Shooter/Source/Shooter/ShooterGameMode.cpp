// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameMode.h"
#include "ShooterHUD.h"
#include "ShooterCharacter.h"
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
}

void AShooterGameMode::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);
}