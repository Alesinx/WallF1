// Fill out your copyright notice in the Description page of Project Settings.



#include "WallF1GameModeInGameBase.h"
#include "WallF1GameInstance.h"
#include "WallF1SensorHandler.h"
#include "Math/UnrealMathUtility.h"

void AWallF1GameModeInGameBase::StartPlay()
{
	CachedGameInstance = GetGameInstance<UWallF1GameInstance>();
	if(!CachedGameInstance)
		UE_LOG(LogTemp, Fatal, TEXT("Could not get WallF1 game instance"));

	CachedSensorHandler = CachedGameInstance->GetSensorHandler();
	if (!CachedSensorHandler)
	{
		UE_LOG(LogTemp, Error, TEXT("CachedSensorHandler is null"));
		return;
	}

	FWallF1SensorColor SensorColor;
	SensorColor.r = 255;
	SensorColor.g = 0;
	SensorColor.b = 0;

	CachedSensorHandler->SetDisplayColor(SensorColor);

	GetWorld()->GetTimerManager().SetTimer(GameStartCountdown, this, &AWallF1GameModeInGameBase::OnCountdownStep, 1, true);
}

void AWallF1GameModeInGameBase::OnCountdownStep()
{
	if(Countdown <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(GameStartCountdown);
		StartGame();
	}
	else
	{
		Countdown--;
		// Update UI

		switch (Countdown)
		{
		case 4:
			CachedSensorHandler->TurnOnLed(6);
			break;
		case 3:
			CachedSensorHandler->TurnOnLed(5);
			break;
		case 2:
			CachedSensorHandler->TurnOnLed(4);
			break;
		case 1:
			CachedSensorHandler->TurnOffAllLeds();
			break;
		}
	}
}

void AWallF1GameModeInGameBase::StartGame()
{
	UE_LOG(LogTemp, Display, TEXT("GAME STARTED"));

	InitializeGameSensorDisplayColor();

	int RndSensor = FMath::RandRange(1, 9);
	CachedSensorHandler->EnableSensorDetection(RndSensor);

	CachedSensorHandler->OnSensorDetection.AddDynamic(this, &AWallF1GameModeInGameBase::DebugSensorDetection);
}

void AWallF1GameModeInGameBase::InitializeGameSensorDisplayColor()
{
	if(!CachedSensorHandler)
	{
		UE_LOG(LogTemp, Error, TEXT("CachedSensorHandler is null"));
		return;
	}

	FWallF1SensorColor SensorColor;
	SensorColor.r = 0;
	SensorColor.g = 255;
	SensorColor.b = 0;
	CachedSensorHandler->SetDisplayColor(SensorColor);
}

void AWallF1GameModeInGameBase::DebugSensorDetection(int SensorId)
{
	UE_LOG(LogTemp, Display, TEXT("DETECTION ON SENSOR WITH ID: %i"), SensorId);

	CachedSensorHandler->DisableSensorDetection(SensorId + 1);
	int RndSensor = FMath::RandRange(1, 9);
	CachedSensorHandler->EnableSensorDetection(RndSensor);
}
