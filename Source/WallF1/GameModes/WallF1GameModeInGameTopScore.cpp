// Fill out your copyright notice in the Description page of Project Settings.


#include "WallF1GameModeInGameTopScore.h"
#include "../WallF1GameInstance.h"

AWallF1GameModeInGameTopScore::AWallF1GameModeInGameTopScore() : Super()
{
	WallF1GameMode = EWallF1GameMode::TOP_SCORE;
}

void AWallF1GameModeInGameTopScore::StartWallF1Game()
{
	UE_LOG(LogTemp, Display, TEXT("TOP-SCORE GAME MODE STARTED"));

	CachedGameInstance->AddNewGameScore(WallF1GameMode, 10);
	CachedGameInstance->AddNewGameScore(WallF1GameMode, 9);
	CachedGameInstance->AddNewGameScore(WallF1GameMode, 11);
	CachedGameInstance->AddNewGameScore(WallF1GameMode, 8);
	Score = 15;
}

void AWallF1GameModeInGameTopScore::HandleSensorDetection(int SensorId)
{
}