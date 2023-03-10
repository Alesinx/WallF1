// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WallF1GameInstance.generated.h"

class UWallF1SensorHandler;
class ULevel;

UENUM(BlueprintType)
enum class EWallF1GameMode : uint8
{
	TOP_SCORE = 0,
	RANDOM = 1,
	PUZZLE = 2,
	WALL = 3,
};

/**
 * WallF1 Game Instance.
 */
UCLASS()
class WALLF1_API UWallF1GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "WallF1 Game Instance")
	void LoadInGameLevel(EWallF1GameMode GameMode);

	UFUNCTION(BlueprintCallable, Category = "WallF1 Game Instance")
	void LoadGameModeSelectionLevel();

	UWallF1SensorHandler* GetSensorHandler() { return SensorHandler; }

	TSubclassOf<UUserWidget> GetGameModeSelectionWidgetClass() { return GameModeSelectionWidgetClass; }

	TSubclassOf<UUserWidget> GetInGameWidgetClass() { return InGameWidgetClass; }

	TArray<uint16> GetGameModeRanking(EWallF1GameMode GameMode);

	/// <summary>
	/// Checks the new score and update the game mode ranking if needed
	/// </summary>
	/// <param name="GameMode"></param>
	/// <param name="InScore"></param>
	/// <returns>True if new score entered the ranking. False is the score was ignored </returns>
	bool AddNewGameScore(EWallF1GameMode GameMode, uint16 InScore);

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
	uint8 MaxRankingSize = 5;

	UPROPERTY()
	UWallF1SensorHandler* SensorHandler;

	UPROPERTY()
	TArray<uint16> TopScoreGameModeScoreRanking;

	UPROPERTY()
	TArray<uint16> RandomGameModeScoreRanking;

	UPROPERTY()
	TArray<uint16> PuzzleGameModeScoreRanking;

	UPROPERTY()
	TArray<uint16> WallGameModeScoreRanking;
};
