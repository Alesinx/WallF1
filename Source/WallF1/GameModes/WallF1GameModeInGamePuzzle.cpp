#include "WallF1GameModeInGamePuzzle.h"
#include "../WallF1GameInstance.h"

AWallF1GameModeInGamePuzzle::AWallF1GameModeInGamePuzzle() : Super()
{
	WallF1GameMode = EWallF1GameMode::PUZZLE;

	// Populate Sensor IDs
	for(int i = 1; i <= 9; ++i)
	{
		SensorsIds.Add((uint8)i);
	}
}

void AWallF1GameModeInGamePuzzle::StartWallF1Game()
{
	UE_LOG(LogTemp, Display, TEXT("PUZZLE GAME MODE STARTED"));

	FWallF1SensorColor green = FWallF1SensorColor();
	green.r = 0;
	green.g = 255;
	green.b = 0;
	CachedSensorHandler->SetDetectionColorOfAllSensors(green);

	TArray<uint8> RandomSensors = GetNRandomSensors(3);
	CachedSensorHandler->EnableSensorDetection(RandomSensors);
	//for(int id : RandomSensors)
	//{
	//	CachedSensorHandler->EnableSensorDetection(id);
	//}
}

void AWallF1GameModeInGamePuzzle::HandleSensorDetection(int SensorId)
{
	IncreaseScore(1);
	CachedSensorHandler->DisableSensorDetection(SensorId);

	if (CachedSensorHandler->AreAllSensorsOff())
	{
		IncreaseScore(3);
		TArray<uint8> RandomSensors = GetNRandomSensors(3);
		CachedSensorHandler->EnableSensorDetection(RandomSensors);
		//for (int id : RandomSensors)
		//{
		//	CachedSensorHandler->EnableSensorDetection(id);
		//}
	}
}

TArray<uint8> AWallF1GameModeInGamePuzzle::GetNRandomSensors(int InN)
{
	// Sort using a custom predicate that returns true/false randomly
	SensorsIds.Sort([this](const int Item1, const int Item2) {
		return FMath::FRand() < 0.5f;
	});

	// Return the first N elements
	TArray<uint8> Out;
	for(int i = 0; i < InN; ++i)
	{
		Out.Add((uint8)SensorsIds[i]);
	}

	return Out;
}
