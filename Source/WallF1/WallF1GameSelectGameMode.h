// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "WallF1GameSelectGameMode.generated.h"

class UUserWidget;
class UWallF1GameInstance;
class UWallF1SensorHandler;

/**
 * 
 */
UCLASS()
class WALLF1_API AWallF1GameSelectGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay() override;

private:
	UWallF1GameInstance* CachedGameInstance;
	UWallF1SensorHandler* CachedSensorHandler;

	UUserWidget* GameModeSelectionWidget;
};
