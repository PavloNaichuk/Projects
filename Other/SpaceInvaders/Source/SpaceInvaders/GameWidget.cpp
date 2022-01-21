// Fill out your copyright notice in the Description page of Project Settings.

#include "GameWidget.h"
#include "SpaceInvaders.h"

void UGameWidget::Load()
{
	const FName TextBlockName = FName(TEXT("GameTextBlock"));
	if (ScoreText == nullptr) 
	{
		ScoreText = (UTextBlock*)(WidgetTree->FindWidget(TextBlockName));
	}
}

void UGameWidget::SetScore(int Score)
{
	if (ScoreText != nullptr) 
	{
		ScoreText->SetText(FText::FromString(FString(TEXT("Score: ")) + FString::FromInt(Score)));
	}
}

void UGameWidget::OnGameOver(int Score)
{
	if (ScoreText != nullptr)
	{
		ScoreText->SetText(FText::FromString(FString(TEXT("Score: ")) + FString::FromInt(Score)+ TEXT("\nPress R to restart")));
	}
}
