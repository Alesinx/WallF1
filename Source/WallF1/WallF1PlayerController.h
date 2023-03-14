// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WallF1PlayerController.generated.h"

class AWallF1GameModeInGameBase;

/**
 * 
 */
UCLASS()
class WALLF1_API AWallF1PlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	AWallF1GameModeInGameBase* CachedGameMode;
	
	virtual void BeginPlay() override;
	virtual bool InputTouch(uint32 InHandle, ETouchType::Type InType, const FVector2D& InTouchLocation, float InForce, FDateTime InDeviceTimestamp, uint32 InTouchpadIndex) override;
};
