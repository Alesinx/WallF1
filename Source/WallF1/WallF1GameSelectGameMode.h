// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "WallF1GameSelectGameMode.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class WALLF1_API AWallF1GameSelectGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> GameModeSelectionWidgetClass;

private:
	UUserWidget* GameModeSelectionWidget;
};
