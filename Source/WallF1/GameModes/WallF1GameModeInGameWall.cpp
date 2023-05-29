#include "WallF1GameModeInGameWall.h"
#include "../WallF1GameInstance.h"

AWallF1GameModeInGameWall::AWallF1GameModeInGameWall() : Super()
{
	WallF1GameMode = EWallF1GameMode::WALL;
}

void AWallF1GameModeInGameWall::StartWallF1Game()
{
	UE_LOG(LogTemp, Display, TEXT("WALL GAME MODE STARTED"));

	FWallF1SensorColor yellow = FWallF1SensorColor();
	yellow.r = 255;
	yellow.g = 255;
	yellow.b = 0;
	CachedSensorHandler->SetDetectionColorOfAllSensors(yellow);
	CachedSensorHandler->EnableAllSensorsDetection();
}

void AWallF1GameModeInGameWall::HandleSensorDetection(int SensorId)
{
	IncreaseScore(1);
	CachedSensorHandler->DisableSensorDetection(SensorId);

	if(CachedSensorHandler->AreAllSensorsOff())
	{
		IncreaseScore(5);
		CachedSensorHandler->EnableAllSensorsDetection();
	}
}