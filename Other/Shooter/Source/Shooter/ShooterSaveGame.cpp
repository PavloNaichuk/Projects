// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterSaveGame.h"

UShooterSaveGame::UShooterSaveGame()
{
	PlayerName = TEXT("TestSaveSlot");
	UserIndex = 0;

	CharacterStats.WeaponName = "";
	CharacterStats.LevelName = "";
}

