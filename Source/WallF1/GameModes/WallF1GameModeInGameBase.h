// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../WallF1SensorHandler.h"
#include "WallF1GameModeInGameBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountdownUpdate, uint8, CurrentCountdown);

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

	UPROPERTY(BlueprintAssignable)
	FOnCountdownUpdate OnCountdownUpdate;

protected:
	uint16 Score = 0;
	FWallF1SensorColor SensorDisplayColor;
	FWallF1SensorColor SensorDetectionColor;
	TSubclassOf<UUserWidget> UIWidgetClass;
	UWallF1GameInstance* CachedGameInstance;
	UWallF1SensorHandler* CachedSensorHandler;
	bool bGameModeInGame = false;
	
	virtual void StartWallF1Game();

	UFUNCTION()
	virtual void HandleSensorDetection(int SensorId);

private:
	UUserWidget* UIWidget;
	uint8 Countdown = 5;
	FTimerHandle GameStartCountdown;

	void StartPlay() override;
	void PlayCountdownAnimation();
	void StartGameModeSelection();

	UFUNCTION()
	void HandleCountdownStep();
};
