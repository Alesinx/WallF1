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
	if (WallF1GameMode == EWallF1GameMode::NONE)
		UIWidgetClass = CachedGameInstance->GetGameModeSelectionWidgetClass();
	else
		UIWidgetClass = CachedGameInstance->GetInGameWidgetClass();
		

	if (!UIWidgetClass)
		UE_LOG(LogTemp, Fatal, TEXT("Could not create UI widget. UI widget class reference was null"));

	UIWidget = CreateWidget(GetWorld(), UIWidgetClass, FName("UI Widget"));
	UIWidget->AddToViewport();

	if (WallF1GameMode == EWallF1GameMode::NONE)
		StartGameModeSelection();
	else
		PlayCountdownAnimation();
}

void AWallF1GameModeInGameBase::PlayCountdownAnimation()
{
	FWallF1SensorColor Red;
	Red.r = 255;
	Red.g = 0;
	Red.b = 0;

	CachedSensorHandler->SetDisplayColor(Red);

	GetWorld()->GetTimerManager().SetTimer(GameStartCountdown, this, &AWallF1GameModeInGameBase::HandleCountdownStep, 1 , true);
}

void AWallF1GameModeInGameBase::StartGameModeSelection()
{
	UE_LOG(LogTemp, Display, TEXT("GAME MODE SELECTION STARTED"));

	CachedSensorHandler->DisableAllSensorsDetection();
	CachedSensorHandler->TurnOffAllLeds();
}

void AWallF1GameModeInGameBase::GameOver()
{
	UE_LOG(LogTemp, Display, TEXT("GAME OVER"));
	RankingPosition = CachedGameInstance->AddNewGameScore(WallF1GameMode, Score);
	OnGameOver.Broadcast();
}

void AWallF1GameModeInGameBase::HandleCountdownStep()
{
	if (Countdown <= 0)
	{
		// Begin
		GetWorld()->GetTimerManager().ClearTimer(GameStartCountdown);
		CachedSensorHandler->SetDisplayColor(SensorDisplayColor);
		CachedSensorHandler->SetDetectionColor(SensorDetectionColor);
		GetWorld()->GetTimerManager().SetTimer(GameStartCountdown, this, &AWallF1GameModeInGameBase::GameOver, CachedGameInstance->GetGameDurationInSeconds(), false);
		StartWallF1Game();
	}
	else
	{
		Countdown--;

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

		OnCountdownUpdate.Broadcast(Countdown);
	}
}

void AWallF1GameModeInGameBase::StartWallF1Game()
{
	UE_LOG(LogTemp, Fatal, TEXT("StartWallF1Game should never be called in in-game mode base"));
}

void AWallF1GameModeInGameBase::HandleSensorDetection(int SensorId)
{
	UE_LOG(LogTemp, Fatal, TEXT("HandleSensorDetection should never be called in in-game mode base"));
}