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
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Wellcome to Bull Cows"));

    mHiddenWord = TEXT("cakes");
    mLives = mHiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), mHiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), mLives);
    PrintLine(TEXT("Type in your guess and \npress enter to continue..."));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(FString Guess)
{
    if (Guess == mHiddenWord)
    {
        PrintLine(TEXT("You have Won!"));
        EndGame();
        return;
    }

    if (Guess.Len() != mHiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word is %i letters long"), mHiddenWord.Len());
        PrintLine(TEXT("Sorry, try guessing again! \nYou have %i lives remaining"), mLives);
        return;
    }
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again"));
        return;
    }

    --mLives;
    PrintLine(TEXT("Lost a  life!"));
    
    if (mLives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have  no lives left!"));
        PrintLine(TEXT("The hidden word was: %s"), *mHiddenWord);

        EndGame();
        return;
    }
    PrintLine(TEXT("Guess again, you have %i lives left"), mLives);
}

bool UBullCowCartridge::IsIsogram(FString Word) const
{
    return true;
}
