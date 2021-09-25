// Fill out your copyright notice in the Description page of Project Settings.


#include "TestWidget.h"

void UTestWidget::Load()
{
	if (ScoreText == nullptr)
	{
		ScoreText = (UTextBlock*)(WidgetTree->FindWidget(FName(TEXT("ScoreTextBlock"))));
	}

	if (WaveNumberText == nullptr)
	{
		WaveNumberText = (UTextBlock*)(WidgetTree->FindWidget(FName(TEXT("WaveNumberTextBlock"))));
	}
}

void UTestWidget::SetScore(int Score)
{
	if (ScoreText != nullptr)
	{
		ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Score)));
	}
}

void UTestWidget::SetWaveNumber(int WaveNumber)
{
	if (WaveNumberText != nullptr)
	{
		WaveNumberText->SetText(FText::FromString(FString::Printf(TEXT("Wave number: %d"), WaveNumber)));
	}
}
