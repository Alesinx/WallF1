// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WallF1GameInstance.generated.h"

class UWallF1SensorHandler;
class ULevel;

USTRUCT()
struct WALLF1_API FWallF1Config
{
	GENERATED_BODY()

	UPROPERTY()
	FString Host;

	UPROPERTY()
	int Port;

	UPROPERTY()
	FString TopicToPublishIn;

	UPROPERTY()
	FString TopicToSubscribeTo;

	UPROPERTY()
	int GameDurationInSeconds = 30;

	UPROPERTY()
	int StandByTimerInSeconds = 60;
};

UENUM(BlueprintType)
enum class EWallF1GameMode : uint8
{
	NONE = 0,
	TOP_SCORE = 1,
	RANDOM = 2,
	PUZZLE = 3,
	WALL = 4,
};

/**
 * WallF1 Game Instance.
 */
UCLASS()
class WALLF1_API UWallF1GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	bool bShowStandbyScreenOnGameModeSelectLoad = true;

	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "WallF1 Game Instance")
	void LoadInGameLevel(EWallF1GameMode GameMode);

	UFUNCTION(BlueprintCallable, Category = "WallF1 Game Instance")
	void LoadGameModeSelectionLevel();

	UFUNCTION(BlueprintCallable, Category = "WallF1 Game Instance")
	const TArray<int>& GetGameModeRanking(EWallF1GameMode GameMode);

	UWallF1SensorHandler* GetSensorHandler() { return SensorHandler; }

	TSubclassOf<UUserWidget> GetGameModeSelectionWidgetClass() { return GameModeSelectionWidgetClass; }

	TSubclassOf<UUserWidget> GetInGameWidgetClass() { return InGameWidgetClass; }

	const FWallF1Config GetWallF1Config() { return WallF1Config; }

	/// <summary>
	/// Checks the new score and update the game mode ranking if needed
	/// </summary>
	/// <param name="GameMode"></param>
	/// <param name="InScore"></param>
	/// <returns> Ranking position if new score entered in it, -1 otherwise </returns>
	int AddNewGameScore(EWallF1GameMode GameMode, int InScore);

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> GameModeSelectionWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InGameWidgetClass;

	UPROPERTY(EditAnywhere)
	FName GameModeSelectionLevelName;

	UPROPERTY(EditAnywhere)
	FName InGameLevelName;

	UPROPERTY(EditAnywhere)
	FString TopScoreGameModeURL;

	UPROPERTY(EditAnywhere)
	FString RandomGameModeURL;

	UPROPERTY(EditAnywhere)
	FString PuzzleGameModeURL;

	UPROPERTY(EditAnywhere)
	FString WallGameModeURL;

private:
	int MaxRankingSize = 5;

	UPROPERTY()
	UWallF1SensorHandler* SensorHandler;

	UPROPERTY()
	TArray<int> TopScoreGameModeScoreRanking;

	UPROPERTY()
	TArray<int> RandomGameModeScoreRanking;

	UPROPERTY()
	TArray<int> PuzzleGameModeScoreRanking;

	UPROPERTY()
	TArray<int> WallGameModeScoreRanking;

	void LoadConfigFile(FWallF1Config& WallF1Config);

	FWallF1Config WallF1Config;
};
