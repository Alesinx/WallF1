// Copyright Epic Games, Inc. All Rights Reserved.


#include "WallF1GameSelectGameMode.h"
#include "Blueprint/WidgetTree.h"

void AWallF1GameSelectGameMode::StartPlay()
{
	Super::StartPlay();
	GameModeSelectionWidget = CreateWidget(GetWorld(), GameModeSelectionWidgetClass, FName("Game Selection widget"));
	GameModeSelectionWidget->AddToViewport();
}
