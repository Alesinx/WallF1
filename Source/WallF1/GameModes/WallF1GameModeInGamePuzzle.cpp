// Fill out your copyright notice in the Description page of Project Settings.


#include "WallF1GameModeInGamePuzzle.h"
#include "../WallF1GameInstance.h"

AWallF1GameModeInGamePuzzle::AWallF1GameModeInGamePuzzle() : Super()
{
	bGameModeInGame = true;
}

void AWallF1GameModeInGamePuzzle::StartWallF1Game()
{
	UE_LOG(LogTemp, Display, TEXT("PUZZLE GAME MODE STARTED"));
}

void AWallF1GameModeInGamePuzzle::HandleSensorDetection(int SensorId)
{
}
