// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UGameOverWidget::Initialize()
{
    Super::Initialize();

    GameOver->OnClicked.AddDynamic(this, &UGameOverWidget::GameOverButtonClicked);

    return true;
}

void UGameOverWidget::GameOverButtonClicked()
{
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
