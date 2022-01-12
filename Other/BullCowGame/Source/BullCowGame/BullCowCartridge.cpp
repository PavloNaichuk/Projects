// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    SetupGame();

    PrintLine(TEXT("The HiddenWord is: %s."), *mHiddenWord); // Debug line

    PrintLine(TEXT("Wellcome to Bull Cows"));
    PrintLine(TEXT("Guess the %i letter word!"), mHiddenWord.Len());
    PrintLine(TEXT("Type in your guess and press enter to continue..."));
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
            PrintLine(TEXT("The Hidden Word is %i characters long, try again!"), mHiddenWord.Len());
        }

        PrintLine(TEXT("You have Lost!"));
    }
}

void UBullCowCartridge::SetupGame()
{
    mHiddenWord = TEXT("cake");
    mLives = 4;
}
