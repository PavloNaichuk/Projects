// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiniFootballGameMode.h"
#include "MiniFootballCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMiniFootballGameMode::AMiniFootballGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
