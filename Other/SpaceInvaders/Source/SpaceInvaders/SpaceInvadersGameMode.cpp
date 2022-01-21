// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceInvadersGameMode.h"
#include "EnemyController.h"
#include "GameWidget.h"

void ASpaceInvadersGameMode::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);
	((UGameWidget*)CurrentWidget)->Load();
}

void ASpaceInvadersGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameTimer += DeltaTime;
	EnemyTimer -= DeltaTime;

	if (EnemyTimer <= 0.0f)
	{
		float DifficultyPercentage = FMath::Min(GameTimer / TIME_TO_MINIMUM_INTERVAL, 1.0f);
		EnemyTimer = MAXIMUM_INTERVAL - (MAXIMUM_INTERVAL - MINIMUM_INTERVAL) * DifficultyPercentage;

		UWorld* World = GetWorld();
		if (World)
		{
			FVector Location = FVector(600.0f, FMath::RandRange(-800.0f, 800.0f), -4420.0f);
			World->SpawnActor<AEnemyController>(EnemyBlueprint, Location, FRotator::ZeroRotator);
		}
	}
}

void ASpaceInvadersGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr) 
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void ASpaceInvadersGameMode::IncrementScore()
{
	Score += 100;
	((UGameWidget*)CurrentWidget)->SetScore(Score);
}

void ASpaceInvadersGameMode::OnGameOver() 
{
	((UGameWidget*)CurrentWidget)->OnGameOver(Score);
}
