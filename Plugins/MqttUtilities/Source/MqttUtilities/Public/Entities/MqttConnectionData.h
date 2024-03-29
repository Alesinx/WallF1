// Copyright (c) 2019 Nineva Studios

#pragma once

#include "MqttConnectionData.generated.h"

USTRUCT(BlueprintType)
struct MQTTUTILITIES_API FMqttConnectionData
{
	GENERATED_BODY()

    /** User login. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MQTT")
	FString Login;

    /** User password. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MQTT")
	FString Password;
};