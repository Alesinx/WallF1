// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../WallF1GameInstance.h"
#include "../WallF1SensorHandler.h"
#include "WallF1GameModeInGameBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountdownUpdate, uint8, CurrentCountdown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);

class UUserWidget;

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

	UPROPERTY(BlueprintAssignable)
	FOnGameOver OnGameOver;

	UPROPERTY(BlueprintReadOnly)
	EWallF1GameMode WallF1GameMode = EWallF1GameMode::NONE;

	UFUNCTION(BlueprintCallable, Category = "WallF1 Game Mode")
	const int GetScore() const { return Score; }

	UFUNCTION(BlueprintCallable, Category = "WallF1 Game Mode")
	const int GetRankingPosition() const { return RankingPosition; }

protected:
	int Score = 0;
	int RankingPosition = -1;
	FWallF1SensorColor SensorDisplayColor;
	FWallF1SensorColor SensorDetectionColor;
	TSubclassOf<UUserWidget> UIWidgetClass;
	UWallF1GameInstance* CachedGameInstance;
	UWallF1SensorHandler* CachedSensorHandler;
	
	UFUNCTION()
	virtual void StartWallF1Game();

	UFUNCTION()
	virtual void HandleSensorDetection(int SensorId);

private:
	UUserWidget* UIWidget;
	uint8 Countdown = 5;
	FTimerHandle GameStartCountdown;
	FTimerHandle GameOverTimer;

	void StartPlay() override;
	void PlayCountdownAnimation();
	void StartGameModeSelection();
	void GameOver();

	UFUNCTION()
	void HandleCountdownStep();
};
