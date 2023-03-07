// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WallF1GameModeInGameBase.generated.h"

class UWallF1GameInstance;
class UWallF1SensorHandler;

/**
 * Base clase for WallF1 games
 */
UCLASS()
class WALLF1_API AWallF1GameModeInGameBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay() override;

	virtual void OnCountdownStep();

	virtual void StartGame();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> GameModeSelectionWidgetClass;
	
	FTimerHandle GameStartCountdown;

	uint8 Countdown = 5;

	UWallF1GameInstance* CachedGameInstance; 
	UWallF1SensorHandler* CachedSensorHandler;

	uint16 Score = 0;

	virtual void InitializeGameSensorDisplayColor();

private:
	UUserWidget* GameModeSelectionWidget;

	UFUNCTION()
	void DebugSensorDetection(int sensorId);

};
