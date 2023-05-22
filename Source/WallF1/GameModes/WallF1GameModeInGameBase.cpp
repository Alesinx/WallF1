#include "WallF1GameModeInGameBase.h"
#include "../WallF1PlayerController.h"
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

	PlayerControllerClass = AWallF1PlayerController::StaticClass();
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

	CachedSensorHandler->SetDefaultDisplayColor(Red);

	GetWorld()->GetTimerManager().SetTimer(GameStartCountdown, this, &AWallF1GameModeInGameBase::HandleCountdownStep, 1, true);
}

void AWallF1GameModeInGameBase::StartGameModeSelection()
{
	UE_LOG(LogTemp, Display, TEXT("GAME MODE SELECTION STARTED"));

	CachedSensorHandler->DisableAllSensorsDetection();
	CachedSensorHandler->TurnOffAllLeds();

	if (CachedGameInstance->bShowStandbyScreenOnGameModeSelectLoad)
	{
		ShowStandyByScreen();

		// Don't show stand-by screen next time game selection is loaded
		CachedGameInstance->bShowStandbyScreenOnGameModeSelectLoad = false;
	}
}

void AWallF1GameModeInGameBase::GameOver()
{
	UE_LOG(LogTemp, Display, TEXT("GAME OVER"));
	RankingPosition = CachedGameInstance->AddNewGameScore(WallF1GameMode, Score);
	OnGameOver.Broadcast();

	CachedSensorHandler->DisableAllSensorsDetection();
	CachedSensorHandler->TurnOffAllLeds();

	CachedSensorHandler->OnSensorDetection.RemoveDynamic(this, &AWallF1GameModeInGameBase::HandleSensorDetection);
}

void AWallF1GameModeInGameBase::ShowStandyByScreen()
{
	if (WallF1GameMode == EWallF1GameMode::NONE)
		OnStandBy.Broadcast();
}

void AWallF1GameModeInGameBase::HandleCountdownStep()
{
	CountdownLeft--;

	if (CountdownLeft <= 0)
	{
		// Begin
		GetWorld()->GetTimerManager().ClearTimer(GameStartCountdown);
		CachedSensorHandler->SetDefaultDisplayColor(SensorDisplayColor);
		CachedSensorHandler->SetDetectionColorOfAllSensors(SensorDetectionColor);
		GetWorld()->GetTimerManager().SetTimer(GameStartCountdown, this, &AWallF1GameModeInGameBase::GameOver, CachedGameInstance->GetWallF1Config().GameDurationInSeconds, false);
		StartWallF1Game();
	}
	else
	{
		switch (CountdownLeft)
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

	OnCountdownUpdate.Broadcast(CountdownLeft);
}

void AWallF1GameModeInGameBase::ResetStandbyTimer()
{
	if (WallF1GameMode == EWallF1GameMode::NONE)
	{
		OnDeviceActive.Broadcast();
		GetWorld()->GetTimerManager().ClearTimer(StandByTimer);

		int StandByTimerInSeconds = CachedGameInstance->GetWallF1Config().StandByTimerInSeconds;
		GetWorld()->GetTimerManager().SetTimer(GameStartCountdown, this, &AWallF1GameModeInGameBase::ShowStandyByScreen, StandByTimerInSeconds, false);
	}
}

void AWallF1GameModeInGameBase::StartWallF1Game()
{
	UE_LOG(LogTemp, Fatal, TEXT("StartWallF1Game should never be called in in-game mode base. Something is wrong"));
}

void AWallF1GameModeInGameBase::HandleSensorDetection(int SensorId)
{
	UE_LOG(LogTemp, Fatal, TEXT("Sensor detection on game selection screen. Detection should not be enable at this point. Something might be wrong"));
}

void AWallF1GameModeInGameBase::IncreaseScore(int InAmount)
{
	Score += InAmount;
	OnScoreIncreased.Broadcast();
}
