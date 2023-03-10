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

TArray<uint16> UWallF1GameInstance::GetGameModeRanking(EWallF1GameMode GameMode)
{
	switch(GameMode)
	{
	case EWallF1GameMode::TOP_SCORE:
		return TopScoreGameModeScoreRanking;
	case EWallF1GameMode::RANDOM:
		return RandomGameModeScoreRanking;
	case EWallF1GameMode::PUZZLE:
		return PuzzleGameModeScoreRanking;
	case EWallF1GameMode::WALL:
		return WallGameModeScoreRanking;
	}
}

bool UWallF1GameInstance::AddNewGameScore(EWallF1GameMode GameMode, uint16 InScore)
{
	TArray<uint16>* TargetScoreArray;
	switch (GameMode)
	{
	case EWallF1GameMode::TOP_SCORE:
		TargetScoreArray = &TopScoreGameModeScoreRanking;
		break;
	case EWallF1GameMode::RANDOM:
		TargetScoreArray = &RandomGameModeScoreRanking;
		break;
	case EWallF1GameMode::PUZZLE:
		TargetScoreArray = &PuzzleGameModeScoreRanking;
		break;
	case EWallF1GameMode::WALL:
		TargetScoreArray = &WallGameModeScoreRanking;
		break;
	}

	if(!TargetScoreArray)
		UE_LOG(LogTemp, Error, TEXT("Could not find TargetArray. Did you add a new game mode and forgot to add a case to the switch?"))

	if(TargetScoreArray->Num() < MaxRankingSize)
	{
		TargetScoreArray->Add(InScore);
		return true;
	}
	else
	{
		for(int i = 0; i < MaxRankingSize; ++i)
		{
			if(InScore > (*TargetScoreArray)[i])
			{
				(*TargetScoreArray)[i] = InScore;
				return true;
			}
		}

		return false;
	}
}
