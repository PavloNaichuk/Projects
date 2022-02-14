// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Protagonist.h"

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ShooterSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UShooterSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UShooterSaveGame();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FCharacterStats CharacterStats;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
}; 
