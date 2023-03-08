// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../WallF1SensorHandler.h"
#include "WallF1GameModeInGameBase.generated.h"

class UUserWidget;
class UWallF1GameInstance;

/**
 * Base clase for WallF1 games
 */
UCLASS()
class WALLF1_API AWallF1GameModeInGameBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AWallF1GameModeInGameBase();

protected:
	uint16 Score = 0;
	FWallF1SensorColor SensorDisplayColor;
	FWallF1SensorColor SensorDetectionColor;
	
	virtual void StartWallF1Game();
	virtual void HandleSensorDetection(int sensorId);

private:
	UUserWidget* UIWidget;
	UWallF1GameInstance* CachedGameInstance;
	UWallF1SensorHandler* CachedSensorHandler;
	uint8 Countdown = 5;
	FTimerHandle GameStartCountdown;

	void StartPlay() override;
	void OnCountdownStep();
	void PlayCountdownAnimation();
};
