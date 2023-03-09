// Fill out your copyright notice in the Description page of Project Settings.


#include "WallF1GameModeInGameRandom.h"
#include "../WallF1GameInstance.h"

AWallF1GameModeInGameRandom::AWallF1GameModeInGameRandom() : Super()
{
	bGameModeInGame = true;
}

void AWallF1GameModeInGameRandom::StartWallF1Game()
{
	UE_LOG(LogTemp, Display, TEXT("RANDOM GAME MODE STARTED"));

	int RndSensor = FMath::RandRange(1, 9);
	CachedSensorHandler->EnableSensorDetection(RndSensor);
}

void AWallF1GameModeInGameRandom::HandleSensorDetection(int SensorId)
{
	UE_LOG(LogTemp, Display, TEXT("DETECTION ON SENSOR WITH ID: %i"), SensorId);

	UE_LOG(LogTemp, Display, TEXT("Disabling detection on sensor: %i"), SensorId + 1);
	CachedSensorHandler->DisableSensorDetection(SensorId + 1);

	int RndSensor = FMath::RandRange(1, 9);
	UE_LOG(LogTemp, Display, TEXT("Enabling detection on sensor: %i"), RndSensor);
	CachedSensorHandler->EnableSensorDetection(RndSensor);
}