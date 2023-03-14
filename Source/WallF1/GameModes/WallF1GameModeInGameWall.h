// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WallF1GameModeInGameBase.h"
#include "WallF1GameModeInGameWall.generated.h"

/**
 * 
 */
UCLASS()
class WALLF1_API AWallF1GameModeInGameWall : public AWallF1GameModeInGameBase
{
	GENERATED_BODY()

public:	
	AWallF1GameModeInGameWall();

	void StartWallF1Game() override;
	void HandleSensorDetection(int SensorId) override;
};
