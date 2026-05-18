// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DSGameModeBase.h"
#include "GameMode/DSGameState.h"

ADSGameModeBase::ADSGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/Character/BP_CharacterTest.BP_CharacterTest_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Game/Character/BP_PlayerController.BP_PlayerController_C"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<ADSGameState> GameStateClassRef(TEXT("/Game/Levels/BP_GameState.BP_GameState_C"));
	if (GameStateClassRef.Class)
	{
		GameStateClass = GameStateClassRef.Class;
	}

}
