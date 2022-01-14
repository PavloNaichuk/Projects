// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver) 
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Wellcome to Bull Cows"));

    mHiddenWord = GetValidWords(Words)[FMath::RandRange(0, GetValidWords(Words).Num() - 1)];
    mLives = mHiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), mHiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), mLives);
    PrintLine(TEXT("Type in your guess and \npress enter to continue..."));
    PrintLine(TEXT("The HiddenWord is: %s."), *mHiddenWord);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
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
        PrintLine(TEXT("No repeating letters, guess again!"));
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

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;   
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if ((Word.Len() >= 4) && (Word.Len() <= 8) && (IsIsogram(Word)))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}
