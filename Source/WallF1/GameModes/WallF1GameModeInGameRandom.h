// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WallF1GameModeInGameBase.h"
#include "WallF1GameModeInGameRandom.generated.h"

/**
 * 
 */
UCLASS()
class WALLF1_API AWallF1GameModeInGameRandom : public AWallF1GameModeInGameBase
{
	GENERATED_BODY()
	
	AWallF1GameModeInGameRandom();

	void StartWallF1Game();
	void HandleSensorDetection(int sensorId);
};
