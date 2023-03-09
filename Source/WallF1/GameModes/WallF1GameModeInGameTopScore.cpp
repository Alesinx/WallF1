// Fill out your copyright notice in the Description page of Project Settings.


#include "WallF1GameModeInGameTopScore.h"
#include "../WallF1GameInstance.h"

AWallF1GameModeInGameTopScore::AWallF1GameModeInGameTopScore() : Super()
{
	bGameModeInGame = true;
}

void AWallF1GameModeInGameTopScore::StartWallF1Game()
{
	UE_LOG(LogTemp, Display, TEXT("TOP-SCORE GAME MODE STARTED"));
}

void AWallF1GameModeInGameTopScore::HandleSensorDetection(int SensorId)
{
}