// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheLastAugust_UE5_1GameMode.h"
#include "TheLastAugust_UE5_1Character.h"
#include "UObject/ConstructorHelpers.h"

ATheLastAugust_UE5_1GameMode::ATheLastAugust_UE5_1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
