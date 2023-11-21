// Copyright Epic Games, Inc. All Rights Reserved.

#include "RunnerProjectGameMode.h"
#include "RunnerProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARunnerProjectGameMode::ARunnerProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
