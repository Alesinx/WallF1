#include "WallF1GameModeInGameTopScore.h"
#include "../WallF1GameInstance.h"

AWallF1GameModeInGameTopScore::AWallF1GameModeInGameTopScore() : Super()
{
	WallF1GameMode = EWallF1GameMode::TOP_SCORE;
}

void AWallF1GameModeInGameTopScore::StartWallF1Game()
{
	UE_LOG(LogTemp, Display, TEXT("TOP SCORE GAME MODE STARTED"));

	FWallF1SensorColor red = FWallF1SensorColor();
	red.r = 255;
	red.g = 0;
	red.b = 0;

	FWallF1SensorColor yellow = FWallF1SensorColor();
	yellow.r = 255;
	yellow.g = 255;
	yellow.b = 0;

	CachedSensorHandler->SetDetectionColorOfAllSensors(yellow);

	CachedSensorHandler->SetSensorDetectionColor(1, red);
	CachedSensorHandler->SetSensorDetectionColor(3, red);
	CachedSensorHandler->SetSensorDetectionColor(7, red);
	CachedSensorHandler->SetSensorDetectionColor(9, red);

	CachedSensorHandler->EnableAllSensorsDetection();
}

void AWallF1GameModeInGameTopScore::HandleSensorDetection(int SensorId)
{
	CachedSensorHandler->DisableSensorDetection(SensorId);
	CachedSensorHandler->EnableSensorDetection(SensorId);

	if(SensorId == 1  || SensorId == 3 || SensorId == 7 || SensorId == 9)
	{
		IncreaseScore(3);
	}
	else
	{
		IncreaseScore(1);
	}
}
