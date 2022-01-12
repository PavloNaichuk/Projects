// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    PrintLine(TEXT("Wellcome to Bull Cows"));
    PrintLine(TEXT("Guess the 4 letter word!")); // Magic Number Remove!
    PrintLine(TEXT("Press enter to continue..."));
    SetupGame(); // Setting Up Game
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ClearScreen();

    if (Input == mHiddenWord)
    {
        PrintLine(TEXT("You have Won!"));
    }
    else
    {
        if (Input.Len() != mHiddenWord.Len())
        {
            PrintLine(TEXT("The Hidden Word is 4 characters long, try again!"));
        }

        PrintLine(TEXT("You have Lost!"));
    }
}

void UBullCowCartridge::SetupGame()
{
    mHiddenWord = TEXT("cake");
    mLives = 4;
}
