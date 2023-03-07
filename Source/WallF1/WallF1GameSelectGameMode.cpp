// Copyright Epic Games, Inc. All Rights Reserved.


#include "WallF1GameSelectGameMode.h"
#include "Blueprint/WidgetTree.h"

void AWallF1GameSelectGameMode::StartPlay()
{
	Super::StartPlay();

	if(!GameModeSelectionWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("GameModeSelectionWidgetClass was null"));
		return;
	}

	GameModeSelectionWidget = CreateWidget(GetWorld(), GameModeSelectionWidgetClass, FName("Game Selection widget"));
	GameModeSelectionWidget->AddToViewport();
}
