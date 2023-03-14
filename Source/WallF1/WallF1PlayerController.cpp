// Fill out your copyright notice in the Description page of Project Settings.


#include "WallF1PlayerController.h"
#include "GameModes/WallF1GameModeInGameBase.h"

void AWallF1PlayerController::BeginPlay()
{
	CachedGameMode = GetWorld()->GetAuthGameMode<AWallF1GameModeInGameBase>();
	if (!CachedGameMode)
		UE_LOG(LogTemp, Fatal, TEXT("Could not get reference to game mode"));

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = false;

	Super::BeginPlay();
}

bool AWallF1PlayerController::InputTouch(uint32 InHandle, ETouchType::Type InType, const FVector2D& InTouchLocation, float InForce, FDateTime InDeviceTimestamp, uint32 InTouchpadIndex)
{
	if (!CachedGameMode)
		UE_LOG(LogTemp, Fatal, TEXT("Game mode reference was null"));

	CachedGameMode->ResetStandbyTimer();

	return Super::InputTouch(InHandle,InType, InTouchLocation, InForce, InDeviceTimestamp, InTouchpadIndex);
}
