#include "WallF1GameModeInGameRandom.h"
#include "../WallF1GameInstance.h"

AWallF1GameModeInGameRandom::AWallF1GameModeInGameRandom() : Super()
{
	WallF1GameMode = EWallF1GameMode::RANDOM;
}

void AWallF1GameModeInGameRandom::StartWallF1Game()
{
	UE_LOG(LogTemp, Display, TEXT("RANDOM GAME MODE STARTED"));

	FWallF1SensorColor blue = FWallF1SensorColor();
	blue.r = 0;
	blue.g = 0;
	blue.b = 255;
	CachedSensorHandler->SetDetectionColorOfAllSensors(blue);
	
	EnableRandomSensor();
}

void AWallF1GameModeInGameRandom::HandleSensorDetection(int SensorId)
{
	UE_LOG(LogTemp, Display, TEXT("DETECTION ON SENSOR WITH ID: %i"), SensorId);
	GetWorld()->GetTimerManager().ClearTimer(DetectionTimeout);
	IncreaseScore(1);
	CachedSensorHandler->DisableAllSensorsDetection();
	EnableRandomSensor();
}

void AWallF1GameModeInGameRandom::EnableRandomSensor()
{
	int RndSensor = FMath::RandRange(1, 9);
	UE_LOG(LogTemp, Display, TEXT("Enabling detection on sensor: %i"), RndSensor);
	CachedSensorHandler->EnableSensorDetection(RndSensor);
	GetWorld()->GetTimerManager().SetTimer(DetectionTimeout, this, &AWallF1GameModeInGameRandom::HandleTimout, 7, false);
}

void AWallF1GameModeInGameRandom::HandleTimout()
{
	CachedSensorHandler->DisableAllSensorsDetection();
	EnableRandomSensor();
}
