// Copyright Epic Games, Inc. All Rights Reserved.


#include "WallF1GameSelectGameMode.h"
#include "Blueprint/WidgetTree.h"
#include "WallF1GameInstance.h"
#include "WallF1SensorHandler.h"

void AWallF1GameSelectGameMode::StartPlay()
{
	Super::StartPlay();

	CachedGameInstance = GetGameInstance<UWallF1GameInstance>();
	if (!CachedGameInstance)
		UE_LOG(LogTemp, Fatal, TEXT("Could not get WallF1 game instance"));

	CachedSensorHandler = CachedGameInstance->GetSensorHandler();
	if (!CachedSensorHandler)
		UE_LOG(LogTemp, Fatal, TEXT("CachedSensorHandler is null"));

	CachedSensorHandler->TurnOffAllLeds();

	if(CachedGameInstance->GetGameModeSelectionWidgetClass())
	{
		GameModeSelectionWidget = CreateWidget(GetWorld(), CachedGameInstance->GetGameModeSelectionWidgetClass(), FName("Game Selection widget"));
		GameModeSelectionWidget->AddToViewport();
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("GameModeSelectionWidgetClass was null"));
		return;
	}
}
