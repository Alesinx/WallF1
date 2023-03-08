// Fill out your copyright notice in the Description page of Project Settings.



#include "WallF1GameModeInGameBase.h"
#include "WallF1GameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "Blueprint/WidgetTree.h"

AWallF1GameModeInGameBase::AWallF1GameModeInGameBase()
{
	SensorDisplayColor.r = 0;
	SensorDisplayColor.g = 0;
	SensorDisplayColor.b = 255;

	SensorDetectionColor.r = 0;
	SensorDetectionColor.g = 255;
	SensorDetectionColor.b = 0;
}

void AWallF1GameModeInGameBase::StartPlay()
{
	CachedGameInstance = GetGameInstance<UWallF1GameInstance>();
	if (!CachedGameInstance)
		UE_LOG(LogTemp, Fatal, TEXT("Could not get WallF1 game instance"));

	CachedSensorHandler = CachedGameInstance->GetSensorHandler();
	if (!CachedSensorHandler)
		UE_LOG(LogTemp, Fatal, TEXT("CachedSensorHandler is null"));

	// Create in-game widget
	if (CachedGameInstance->GetInGameWidgetClass())
	{
		UIWidget = CreateWidget(GetWorld(), CachedGameInstance->GetInGameWidgetClass(), FName("UI Widget"));
		UIWidget->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not create UI widget. UI widget class reference was null"));
	}
}

void AWallF1GameModeInGameBase::PlayCountdownAnimation()
{
	FWallF1SensorColor SensorColor;
	SensorColor.r = 255;
	SensorColor.g = 0;
	SensorColor.b = 0;

	CachedSensorHandler->SetDisplayColor(SensorColor);

	GetWorld()->GetTimerManager().SetTimer(GameStartCountdown, this, &AWallF1GameModeInGameBase::OnCountdownStep, 1, true);
}

void AWallF1GameModeInGameBase::OnCountdownStep()
{
	if (Countdown <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(GameStartCountdown);
		CachedSensorHandler->SetDisplayColor(SensorDisplayColor);
		CachedSensorHandler->SetDetectionColor(SensorDetectionColor);
		StartWallF1Game();
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

void AWallF1GameModeInGameBase::StartWallF1Game()
{
	UE_LOG(LogTemp, Display, TEXT("GAME STARTED"));

	int RndSensor = FMath::RandRange(1, 9);
	CachedSensorHandler->EnableSensorDetection(RndSensor);

	CachedSensorHandler->OnSensorDetection.AddDynamic(this, &AWallF1GameModeInGameBase::HandleSensorDetection);
}

void AWallF1GameModeInGameBase::HandleSensorDetection(int SensorId)
{
	UE_LOG(LogTemp, Display, TEXT("DETECTION ON SENSOR WITH ID: %i"), SensorId);

	UE_LOG(LogTemp, Display, TEXT("Disabling detection on sensor: %i"), SensorId + 1);
	CachedSensorHandler->DisableSensorDetection(SensorId + 1);

	int RndSensor = FMath::RandRange(1, 9);
	UE_LOG(LogTemp, Display, TEXT("Enabling detection on sensor: %i"), RndSensor);
	CachedSensorHandler->EnableSensorDetection(RndSensor);
}