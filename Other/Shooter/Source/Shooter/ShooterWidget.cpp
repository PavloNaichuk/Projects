// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterWidget.h"

void UShooterWidget::Load()
{
	if (ScoreText == nullptr)
	{
		ScoreText = (UTextBlock*)(WidgetTree->FindWidget(FName(TEXT("ScoreTextBlock"))));
	}
	if (WaveText == nullptr)
	{
		WaveText = (UTextBlock*)(WidgetTree->FindWidget(FName(TEXT("WaveTextBlock"))));
	}
}

void UShooterWidget::SetScore(int Score)
{
	if (ScoreText != nullptr)
	{
		ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Score)));
	}
}

void UShooterWidget::SetWaveNumber(int WaveNumber)
{
	if (WaveText != nullptr)
	{
		WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), WaveNumber)));
	}
}


