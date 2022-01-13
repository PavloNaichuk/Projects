// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    SetupGame();

    PrintLine(TEXT("The HiddenWord is: %s."), *mHiddenWord); // Debug line
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver) 
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        if (Input == mHiddenWord)
        {
            PrintLine(TEXT("You have Won!"));
            EndGame();
        }
        else
        {
            if (Input.Len() != mHiddenWord.Len())
            {
                PrintLine(TEXT("The Hidden Word is %i characters long. \nYou have lost!"), mHiddenWord.Len());
                EndGame();
            }
        }

    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Wellcome to Bull Cows"));

    mHiddenWord = TEXT("cake");
    mLives = 4;
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), mHiddenWord.Len());
    PrintLine(TEXT("Type in your guess. \nPress enter to continue..."));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again."));
}
