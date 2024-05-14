// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReverbSystemGameMode.h"
#include "ReverbSystemCharacter.h"
#include "UObject/ConstructorHelpers.h"

AReverbSystemGameMode::AReverbSystemGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
