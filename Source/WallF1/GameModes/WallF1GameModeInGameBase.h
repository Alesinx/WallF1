// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../WallF1GameInstance.h"
#include "../WallF1SensorHandler.h"
#include "WallF1GameModeInGameBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountdownUpdate, uint8, CurrentCountdown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScoreIncreased);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStandBy);
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
	FOnStandBy OnStandBy;

	UPROPERTY(BlueprintAssignable)
	FOnCountdownUpdate OnCountdownUpdate;

	UPROPERTY(BlueprintAssignable)
	FOnScoreIncreased OnScoreIncreased;

	UPROPERTY(BlueprintAssignable)
	FOnGameOver OnGameOver;

	UPROPERTY(BlueprintReadOnly)
	EWallF1GameMode WallF1GameMode = EWallF1GameMode::NONE;

	UFUNCTION(BlueprintCallable, Category = "WallF1 Game Mode")
	const int GetScore() const { return Score; }

	UFUNCTION(BlueprintCallable, Category = "WallF1 Game Mode")
	const int GetRankingPosition() const { return RankingPosition; }

	UFUNCTION(BlueprintCallable, Category = "WallF1 Game Mode")
	void ResetStandbyTimer();

protected:
	FWallF1SensorColor SensorDisplayColor;
	FWallF1SensorColor SensorDetectionColor;
	TSubclassOf<UUserWidget> UIWidgetClass;
	UWallF1GameInstance* CachedGameInstance;
	UWallF1SensorHandler* CachedSensorHandler;
	
	UFUNCTION()
	virtual void StartWallF1Game();

	UFUNCTION()
	virtual void HandleSensorDetection(int SensorId);

	void IncreaseScore(int InAmount);

private:
	const uint8 InitialCoundown = 5;
	uint8 CountdownLeft = InitialCoundown;
	
	UUserWidget* UIWidget;
	int Score = 0;
	int RankingPosition = -1;
	FTimerHandle GameStartCountdown;
	FTimerHandle GameOverTimer;
	FTimerHandle StandByTimer;

	void StartPlay() override;
	void PlayCountdownAnimation();
	void StartGameModeSelection();
	void GameOver();
	void ShowStandyByScreen();

	UFUNCTION()
	void HandleCountdownStep();
};
