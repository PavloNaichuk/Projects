// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtagonistPlayerController.h"
#include "Blueprint/UserWidget.h"


AProtagonistPlayerController::AProtagonistPlayerController()
{
	bPauseMenuOpen = false;
}

void AProtagonistPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if (WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		if (EnemyHealthBar)
		{
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
		FVector2D Alignment(0.0f, 0.0f);
		EnemyHealthBar->SetAlignmentInViewport(Alignment);
	}

	if (WPauseMenu)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
		if (PauseMenu)
		{
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AProtagonistPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void AProtagonistPlayerController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AProtagonistPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyHealthBar)
	{
		FVector2D PositionInViewport;
		ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);
		PositionInViewport.Y -= 85.0f;

		FVector2D SizeInViewport(300.0f, 25.0f);

		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
	}
}

void AProtagonistPlayerController::DisplayPauseMenu_Implementation()
{
	bPauseMenuOpen = true;
	bShowMouseCursor = true;
	PauseMenu->SetVisibility(ESlateVisibility::Visible);

	FInputModeGameAndUI InputModeGameAndUI;
	SetInputMode(InputModeGameAndUI);
}


void AProtagonistPlayerController::RemovePauseMenu_Implementation()
{
	bPauseMenuOpen = false;
	bShowMouseCursor = false;
	GameModeOnly();
}

void AProtagonistPlayerController::TogglePauseMenu()
{
	if (!bPauseMenuOpen)
	{
		DisplayPauseMenu();
	}
	else
	{
		RemovePauseMenu();
	}
}

void AProtagonistPlayerController::GameModeOnly()
{
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}