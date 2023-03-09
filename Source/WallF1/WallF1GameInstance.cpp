// Fill out your copyright notice in the Description page of Project Settings.


#include "WallF1GameInstance.h"
#include "WallF1SensorHandler.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Level.h"

void UWallF1GameInstance::Init()
{
	Super::Init();

	SensorHandler = NewObject<UWallF1SensorHandler>();

	SensorHandler->Initialize();
}

void UWallF1GameInstance::LoadInGameLevel(EWallF1GameMode GameMode)
{
	FString Url = "Game=";
	switch(GameMode)
	{
	case EWallF1GameMode::TOP_SCORE:
		Url.Append(TopScoreGameModeURL);
		break;
	case EWallF1GameMode::RANDOM:
		Url.Append(RandomGameModeURL);
		break;
	case EWallF1GameMode::PUZZLE:
		Url.Append(PuzzleGameModeURL);
		break;
	case EWallF1GameMode::WALL:
		Url.Append(WallGameModeURL);
		break;
	}

	UE_LOG(LogTemp, Display, TEXT("LOADING %s LEVEL WITH URL: %s"), *InGameLevelName.ToString(), *Url)
	UGameplayStatics::OpenLevel(this, InGameLevelName, true, Url);
}

void UWallF1GameInstance::LoadGameModeSelectionLevel()
{
	UE_LOG(LogTemp, Display, TEXT("LOADING %s LEVEL"), *GameModeSelectionLevelName.ToString())
	UGameplayStatics::OpenLevel(this, GameModeSelectionLevelName);
}
