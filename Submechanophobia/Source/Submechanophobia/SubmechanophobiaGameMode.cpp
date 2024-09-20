// Copyright Epic Games, Inc. All Rights Reserved.

#include "SubmechanophobiaGameMode.h"
#include "SubmechanophobiaCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASubmechanophobiaGameMode::ASubmechanophobiaGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
