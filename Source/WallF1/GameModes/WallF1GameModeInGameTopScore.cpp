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
	CachedSensorHandler->TurnOnLed(5, red);

	FWallF1SensorColor orange = FWallF1SensorColor();
	orange.r = 255;
	orange.g = 100;
	orange.b = 0;
	CachedSensorHandler->TurnOnLed(4, orange);
	CachedSensorHandler->TurnOnLed(6, orange);
	CachedSensorHandler->TurnOnLed(2, orange);
	CachedSensorHandler->TurnOnLed(8, orange);

	FWallF1SensorColor yellow = FWallF1SensorColor();
	yellow.r = 255;
	yellow.g = 255;
	yellow.b = 0;
	CachedSensorHandler->TurnOnLed(1, yellow);
	CachedSensorHandler->TurnOnLed(3, yellow);
	CachedSensorHandler->TurnOnLed(7, yellow);
	CachedSensorHandler->TurnOnLed(9, yellow);
}

void AWallF1GameModeInGameTopScore::HandleSensorDetection(int SensorId)
{
}