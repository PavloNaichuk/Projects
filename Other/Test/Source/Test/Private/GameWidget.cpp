// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidget.h"

void UGameWidget::Load() 
{
	if (ScoreText == nullptr) 
	{
		ScoreText = (UTextBlock*)(WidgetTree->FindWidget(FName(TEXT("ScoreTextBlock"))));
	}

	if (WaveCountText == nullptr)
	{
		WaveCountText = (UTextBlock*)(WidgetTree->FindWidget(FName(TEXT("WaveCountTextBlock"))));
	}
}

void UGameWidget::SetScore(int Score)
{
	if (ScoreText != nullptr)
	{
		ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Score)));
	}
}

void UGameWidget::SetWaveCount(int WaveCount)
{
	if (WaveCountText != nullptr)
	{
		WaveCountText->SetText(FText::FromString(FString::Printf(TEXT("WaveCount: %d"), WaveCount)));
	}
}
