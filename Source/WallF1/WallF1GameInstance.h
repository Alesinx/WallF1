// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WallF1GameInstance.generated.h"

class UWallF1SensorHandler;

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

	UFUNCTION(BlueprintCallable, Category = "WallF1 sensors")
	UWallF1SensorHandler* GetSensorHandler() { return SensorHandler; }

protected:
	UWallF1SensorHandler* SensorHandler;

	UFUNCTION()
	void DebugSensorDetection(int sensorId);
};
