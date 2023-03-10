// Fill out your copyright notice in the Description page of Project Settings.


#include "WallF1GameModeInGameWall.h"
#include "../WallF1GameInstance.h"

AWallF1GameModeInGameWall::AWallF1GameModeInGameWall() : Super()
{
	WallF1GameMode = EWallF1GameMode::WALL;
}

void AWallF1GameModeInGameWall::StartWallF1Game()
{
	UE_LOG(LogTemp, Display, TEXT("WALL GAME MODE STARTED"));
}

void AWallF1GameModeInGameWall::HandleSensorDetection(int SensorId)
{
}