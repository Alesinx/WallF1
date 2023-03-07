// Fill out your copyright notice in the Description page of Project Settings.


#include "WallF1GameInstance.h"
#include "WallF1SensorHandler.h"

void UWallF1GameInstance::Init()
{
	Super::Init();

	SensorHandler = NewObject<UWallF1SensorHandler>();

	SensorHandler->Initialize();

	//SensorHandler->EnableSensorDetection(1);

	//SensorHandler->OnSensorDetection.AddDynamic(this, &UWallF1GameInstance::DebugSensorDetection);
}

void UWallF1GameInstance::DebugSensorDetection(int SensorId)
{
	UE_LOG(LogTemp, Display, TEXT("DETECTION ON SENSOR WITH ID: %i"), SensorId);
}
