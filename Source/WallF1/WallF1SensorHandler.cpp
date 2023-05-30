#include "WallF1SensorHandler.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "MqttActor.h"

FWallF1SensorColor UWallF1SensorHandler::DefaultDisplayColor = FWallF1SensorColor();

void UWallF1SensorHandler::Initialize(FWallF1Config InConfig)
{
	WallF1Config = InConfig;

	initialized = true;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = TEXT("Mqtt actor");
	MqttActor = NewObject<UMqttActor>();

	if (MqttActor)
	{
		FString clientID = TEXT("client");
		MqttActor->CreateClient(clientID, WallF1Config.Host, WallF1Config.Port);

		MqttActor->OnMessageReceivedNativeDelegate.AddDynamic(this, &UWallF1SensorHandler::OnMessageReceived);
		MqttActor->OnConnectedDelegate.AddDynamic(this, &UWallF1SensorHandler::OnClientConnected);

		MqttActor->Connect();
	}
}

void UWallF1SensorHandler::OnClientConnected()
{
	if(MqttActor)
	{
		MqttActor->Subscribe(WallF1Config.TopicToSubscribeTo, WallF1Config.QoS);
		ready = true;
	}
}

void UWallF1SensorHandler::EnableSensorDetection(uint8 SensorId)
{
	const FString PayloadString = FString::Printf(TEXT("{\"modo\":0,\"idSensor\":%i}"), SensorId);
	QueueMessage(PayloadString);

	SensorsState[SensorId - 1] = EWallF1SensorState::DETECTION_ENABLED;
}

void UWallF1SensorHandler::EnableSensorDetection(TArray<uint8> SensorIds)
{
	if (!SensorIds.IsEmpty())
	{
		FString idList = "";
		for (uint8 id : SensorIds)
		{
			if (!idList.IsEmpty())
			{
				idList.Append(",");
			}
			idList.Append(FString::Printf(TEXT("%d"), id));

			SensorsState[id - 1] = EWallF1SensorState::DETECTION_ENABLED;
		}

		const FString PayloadString = FString::Printf(TEXT("{\"modo\":0,\"sensor\":[%s]}"), *idList);
		QueueMessage(PayloadString);
	}
}

void UWallF1SensorHandler::DisableSensorDetection(uint8 SensorId)
{
	const FString PayloadString = FString::Printf(TEXT("{\"modo\":1,\"idSensor\":%i}"), SensorId);
	QueueMessage(PayloadString);

	SensorsState[SensorId - 1] = EWallF1SensorState::OFF;
}

void UWallF1SensorHandler::DisableSensorDetection(TArray<uint8> SensorIds)
{
}

void UWallF1SensorHandler::TurnOnLed(uint8 SensorId, FWallF1SensorColor InColor)
{
	const FString PayloadString = FString::Printf(TEXT("{\"modo\":2,\"idSensor\":%i,\"r\":%i,\"g\":%i,\"b\":%i}"), SensorId, InColor.r, InColor.g, InColor.b);
	QueueMessage(PayloadString);

	SensorsState[SensorId - 1] = EWallF1SensorState::LED_ON;
}

void UWallF1SensorHandler::TurnOnLed(TArray<uint8> SensorIds, FWallF1SensorColor InColor)
{
}

void UWallF1SensorHandler::TurnOffLed(uint8 SensorId)
{
	const FString PayloadString = FString::Printf(TEXT("{\"modo\":2,\"idSensor\":%i,\"r\":0,\"g\":0,\"b\":0}"), SensorId);
	QueueMessage(PayloadString);

	SensorsState[SensorId - 1] = EWallF1SensorState::OFF;
}

void UWallF1SensorHandler::TurnOffLed(TArray<uint8> SensorIds)
{
}


void UWallF1SensorHandler::EnableAllSensorsDetection()
{
	const FString PayloadString = FString::Printf(TEXT("{\"modo\":0,\"idSensor\":0}"));
	QueueMessage(PayloadString);

	for (int i = 0; i < 9; ++i)
	{
		SensorsState[i] = EWallF1SensorState::DETECTION_ENABLED;
	}
}

void UWallF1SensorHandler::DisableAllSensorsDetection()
{
	const FString PayloadString = FString::Printf(TEXT("{\"modo\":1,\"idSensor\":0}"));
	QueueMessage(PayloadString);

	for (int i = 0; i < 9; ++i)
	{
		SensorsState[i] = EWallF1SensorState::OFF;
	}
}

void UWallF1SensorHandler::TurnOnAllLeds(FWallF1SensorColor InColor)
{
	const FString PayloadString = FString::Printf(TEXT("{\"modo\":2,\"idSensor\":0,\"r\":%i,\"g\":%i,\"b\":%i}"), InColor.r, InColor.g, InColor.b);
	QueueMessage(PayloadString);

	for (int i = 0; i < 9; ++i)
	{
		SensorsState[i] = EWallF1SensorState::LED_ON;
	}
}

void UWallF1SensorHandler::TurnOffAllLeds()
{
	const FString PayloadString = FString::Printf(TEXT("{\"modo\":2,\"idSensor\":0,\"r\":0,\"g\":0,\"b\":0}"));
	QueueMessage(PayloadString);

	for (int i = 0; i < 9; ++i)
	{
		SensorsState[i] = EWallF1SensorState::OFF;
	}
}

void UWallF1SensorHandler::SetDefaultDisplayColor(FWallF1SensorColor InColor)
{
	UWallF1SensorHandler::DefaultDisplayColor = InColor;
}

void UWallF1SensorHandler::SetDetectionColorOfAllSensors(FWallF1SensorColor InColor)
{
	const FString PayloadString = FString::Printf(TEXT("{\"modo\":3,\"idSensor\":0,\"r\":%i,\"g\":%i,\"b\":%i}"), InColor.r, InColor.g, InColor.b);
	QueueMessage(PayloadString);
}

void UWallF1SensorHandler::SetSensorDetectionColor(uint8 SensorId, FWallF1SensorColor InColor)
{
	const FString PayloadString = FString::Printf(TEXT("{\"modo\":3,\"idSensor\":%i,\"r\":%i,\"g\":%i,\"b\":%i}"), SensorId, InColor.r, InColor.g, InColor.b);
	QueueMessage(PayloadString);
}

bool UWallF1SensorHandler::AreAllSensorsOff()
{
	for (int i = 0; i < 9; ++i)
	{
		if (SensorsState[i] != EWallF1SensorState::OFF)
		{
			return false;
		}
	}
	return true;
}

void UWallF1SensorHandler::Tick(float DeltaTime)
{
	if (initialized)
	{
		PurgePendingMessageQueue();
		TrytoPublishPendingMessage();
	}
}

void UWallF1SensorHandler::QueueMessage(const FString& PayloadString)
{
	if(WaitForACKs)
	{
		UE_LOG(LogTemp, Display, TEXT("REQUESTING MESSAGE PUBLISH: %s"), *PayloadString);

		FWallF1PendingMessage PendingMessage;
		PendingMessage.Payload = PayloadString;
		PendingMessage.bPublishRequested = false;
		PendingMessage.bAcknowledged = false;
		PendingMessage.TimeStamp = FDateTime::UtcNow().ToUnixTimestamp();
		PendingMessageQueue.Add(PendingMessage);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("ACTUALLY PUBLISHING MESSAGE: %s"), *PayloadString);
		MqttActor->Publish(WallF1Config.TopicToPublishIn, PayloadString, WallF1Config.QoS);
	}
}

void UWallF1SensorHandler::OnMessageReceived(FString Payload)
{
	UE_LOG(LogTemp, Display, TEXT("MESSAGE RECEIVED: %s"), *Payload);

	bool bIsACK = Payload.Contains("ACK");
	if (bIsACK)
	{
		if(WaitForACKs)
		{
			HandleACKReceived();
		}
		//else  ignore the message
	}
	else
	{
		// Parse json message
		FWallF1SensorResponse SensorResponse;
		FJsonObjectConverter::JsonObjectStringToUStruct(Payload, &SensorResponse);

		// Broadcast delegate if detection is enabled (filter out detections on sensor that are pending to be disabled)
		if (SensorsState[SensorResponse.idSensor - 1] == EWallF1SensorState::DETECTION_ENABLED) 
		{
			OnSensorDetection.Broadcast(SensorResponse.idSensor);
		}
	}
}

void UWallF1SensorHandler::HandleACKReceived()
{
	if (WaitForACKs)
	{
		if (PendingMessageQueue.IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("Recived ACK but pending message queue is empty!"))
				return;
		}

		PendingMessageQueue[0].bAcknowledged = true;
	}
}

void UWallF1SensorHandler::PurgePendingMessageQueue()
{
	// Remove any acknowledged or expired message
	if (ready && WaitForACKs && !PendingMessageQueue.IsEmpty())
	{
		if (PendingMessageQueue[0].bAcknowledged)
		{
			if (!PendingMessageQueue[0].bPublishRequested)
			{
				UE_LOG(LogTemp, Error, TEXT("Not published pending message was acknowledged. State of games is unreliable"))
			}

			PendingMessageQueue.RemoveAt(0);
		}
		else if (FDateTime::UtcNow().ToUnixTimestamp() - PendingMessageQueue[0].TimeStamp >= 2000)
		{
			UE_LOG(LogTemp, Error, TEXT("Pending message acknowledgment expired. State of games is unreliable"))
			PendingMessageQueue.RemoveAt(0);
		}
	}
}

void UWallF1SensorHandler::TrytoPublishPendingMessage()
{
	if (ready && !PendingMessageQueue.IsEmpty() && !PendingMessageQueue[0].bPublishRequested)
	{
		const FString PayloadString = PendingMessageQueue[0].Payload;

		UE_LOG(LogTemp, Display, TEXT("ACTUALLY PUBLISHING MESSAGE: %s"), *PayloadString);

		MqttActor->Publish(WallF1Config.TopicToPublishIn, PayloadString, WallF1Config.QoS);
		if(WaitForACKs)
		{
			PendingMessageQueue[0].bPublishRequested = true;
		}
	}
}
