// Fill out your copyright notice in the Description page of Project Settings.



#include "WallF1GameModeInGameBase.h"
#include "../WallF1GameInstance.h"
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
	Super::StartPlay();

	// Set cached references
	CachedGameInstance = GetGameInstance<UWallF1GameInstance>();
	if (!CachedGameInstance)
		UE_LOG(LogTemp, Fatal, TEXT("Could not get WallF1 game instance"));

	CachedSensorHandler = CachedGameInstance->GetSensorHandler();
	if (!CachedSensorHandler)
		UE_LOG(LogTemp, Fatal, TEXT("CachedSensorHandler is null"));

	// Bind handle sensor detection
	CachedSensorHandler->OnSensorDetection.AddDynamic(this, &AWallF1GameModeInGameBase::HandleSensorDetection);

	// Create UI widget
	if (bGameModeInGame)
		UIWidgetClass = CachedGameInstance->GetInGameWidgetClass();
	else
		UIWidgetClass = CachedGameInstance->GetGameModeSelectionWidgetClass();

	if (!UIWidgetClass)
		UE_LOG(LogTemp, Fatal, TEXT("Could not create UI widget. UI widget class reference was null"));

	UIWidget = CreateWidget(GetWorld(), UIWidgetClass, FName("UI Widget"));
	UIWidget->AddToViewport();

	if (bGameModeInGame)
		PlayCountdownAnimation();
	else
		StartGameModeSelection();
}

void AWallF1GameModeInGameBase::PlayCountdownAnimation()
{
	FWallF1SensorColor SensorColor;
	SensorColor.r = 255;
	SensorColor.g = 0;
	SensorColor.b = 0;

	CachedSensorHandler->SetDisplayColor(SensorColor);

	GetWorld()->GetTimerManager().SetTimer(GameStartCountdown, this, &AWallF1GameModeInGameBase::HandleCountdownStep, 1, true);
}

void AWallF1GameModeInGameBase::StartGameModeSelection()
{
	UE_LOG(LogTemp, Display, TEXT("GAME MODE SELECTION STARTED"));

	CachedSensorHandler->DisableAllSensorsDetection();
	CachedSensorHandler->TurnOffAllLeds();
}

void AWallF1GameModeInGameBase::HandleCountdownStep()
{
	if (Countdown <= 0)
	{
		// Begin
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

		OnCountdownStep(Countdown);
	}
}

void AWallF1GameModeInGameBase::StartWallF1Game()
{
}

void AWallF1GameModeInGameBase::HandleSensorDetection(int SensorId)
{
}