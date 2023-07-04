// Copyright Epic Games, Inc. All Rights Reserved.

#include "CMasMas_ProyectoGameMode.h"
#include "CMasMas_ProyectoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACMasMas_ProyectoGameMode::ACMasMas_ProyectoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
