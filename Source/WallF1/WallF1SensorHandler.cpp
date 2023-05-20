#include "WallF1SensorHandler.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "MqttActor.h"

FWallF1SensorColor UWallF1SensorHandler::DefaultDisplayColor = FWallF1SensorColor();

void UWallF1SensorHandler::Initialize(FWallF1Config InConfig)
{
	WallF1Config = InConfig;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = TEXT("Mqtt actor");
	//MqttActor = GetWorld()->SpawnActor<AMqttActor>(SpawnParams);
	MqttActor = NewObject<AMqttActor>();

	if (MqttActor)
	{
		FString clientID = TEXT("client");
		MqttActor->CreateClient(clientID, WallF1Config.Host, WallF1Config.Port);

		MqttActor->Connect();
	}
	
	//FMQTTURL mqttUrl;
	//mqttUrl.Host = WallF1Config.Host;
	//InConfig.Port = WallF1Config.Port;

	//MqttClient = UMQTTSubsystem::GetOrCreateClient(this, mqttUrl);

	//BPConnect();
	//BPSubscribe(WallF1Config.TopicToSubscribeTo, static_cast<EMQTTQualityOfService>(WallF1Config.QoS));

	//WallF1Config = InConfig;
	//FMqttClientConfig config;
	//config.HostUrl = WallF1Config.Host;
	//config.Port = WallF1Config.Port;
	//config.ClientId = "User";
	//config.EventLoopDeltaMs = -1;
	//MqttClient = UMqttUtilitiesBPL::CreateMqttClient(config);
	//if (!MqttClient)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("MqttClient could not be initialized"));
	//	return;
	//}

	//FMqttConnectionData ConnectionData;
	//ConnectionData.Login = "";
	//ConnectionData.Password = "";

	//ConnectDelegate.BindUFunction(this, FName("OnClientConnected"));
	//MqttClient->Connect(ConnectionData, ConnectDelegate);

	//MessagePublishDelegate.BindUFunction(this, FName("OnMessagePublished"));

	//MqttClient->SetOnPublishHandler(MessagePublishDelegate);
	//SubscribeDelegate.BindUFunction(this, FName("OnSubscribed"));
	//MqttClient->SetOnSubscribeHandler(SubscribeDelegate);
	//MqttClient->Subscribe(WallF1Config.TopicToSubscribeTo, 0);

	//MessageReceivedDelegate.BindUFunction(this, FName("OnMessageReceived"));
	//MqttClient->SetOnMessageHandler(MessageReceivedDelegate);
}

void UWallF1SensorHandler::EnableSensorDetection(uint8 SensorId)
{
	//if (!MqttClient)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized"));
	//	return;
	//}

	const FString PayloadString = FString::Printf(TEXT("{\"modo\":0,\"idSensor\":%i}"), SensorId);
	QueueMessage(PayloadString);

	SensorsState[SensorId - 1] = EWallF1SensorState::DETECTION_ENABLED;
}

void UWallF1SensorHandler::DisableSensorDetection(uint8 SensorId)
{
	//if (!MqttClient)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized"));
	//	return;
	//}

	const FString PayloadString = FString::Printf(TEXT("{\"modo\":1,\"idSensor\":%i}"), SensorId);
	QueueMessage(PayloadString);

	SensorsState[SensorId - 1] = EWallF1SensorState::OFF;
}

void UWallF1SensorHandler::TurnOnLed(uint8 SensorId, FWallF1SensorColor InColor)
{
	if (!MqttActor)
	{
		UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized"));
		return;
	}

	const FString PayloadString = FString::Printf(TEXT("{\"modo\":2,\"idSensor\":%i,\"r\":%i,\"g\":%i,\"b\":%i}"), SensorId, InColor.r, InColor.g, InColor.b);
	QueueMessage(PayloadString);

	SensorsState[SensorId - 1] = EWallF1SensorState::LED_ON;
}

void UWallF1SensorHandler::TurnOffLed(uint8 SensorId)
{
	//if (!MqttClient)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized"));
	//	return;
	//}

	const FString PayloadString = FString::Printf(TEXT("{\"modo\":2,\"idSensor\":%i,\"r\":0,\"g\":0,\"b\":0}"), SensorId);
	QueueMessage(PayloadString);

	SensorsState[SensorId - 1] = EWallF1SensorState::OFF;
}

void UWallF1SensorHandler::EnableAllSensorsDetection()
{
	//if (!MqttClient)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized"));
	//	return;
	//}

	const FString PayloadString = FString::Printf(TEXT("{\"modo\":0,\"idSensor\":0}"));
	QueueMessage(PayloadString);

	for (int i = 0; i < 9; ++i)
	{
		SensorsState[i] = EWallF1SensorState::OFF;
	}
}

void UWallF1SensorHandler::DisableAllSensorsDetection()
{
	//if (!MqttClient)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized"));
	//	return;
	//}

	const FString PayloadString = FString::Printf(TEXT("{\"modo\":1,\"idSensor\":0}"));
	QueueMessage(PayloadString);

	for (int i = 0; i < 9; ++i)
	{
		SensorsState[i] = EWallF1SensorState::OFF;
	}
}

void UWallF1SensorHandler::TurnOnAllLeds(FWallF1SensorColor InColor)
{
	//if (!MqttClient)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized"));
	//	return;
	//}

	const FString PayloadString = FString::Printf(TEXT("{\"modo\":2,\"idSensor\":0,\"r\":%i,\"g\":%i,\"b\":%i}"), InColor.r, InColor.g, InColor.b);
	QueueMessage(PayloadString);

	for (int i = 0; i < 9; ++i)
	{
		SensorsState[i] = EWallF1SensorState::LED_ON;
	}
}

void UWallF1SensorHandler::TurnOffAllLeds()
{

	//if (!MqttClient)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized"));
	//	return;
	//}

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
	//if (!MqttClient)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized"));
	//	return;
	//}

	const FString PayloadString = FString::Printf(TEXT("{\"modo\":3,\"idSensor\":0,\"r\":%i,\"g\":%i,\"b\":%i}"), InColor.r, InColor.g, InColor.b);
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
	// Remove any acknowledged or expired message
	if (!PendingMessageQueue.IsEmpty())
	{
		if (PendingMessageQueue[0].bAcknowledged)
		{
			if (!PendingMessageQueue[0].bPublishRequested)
				UE_LOG(LogTemp, Fatal, TEXT("Not published pending message was acknowledged. State of games is unreliable"))

				// Dequeue
				PendingMessageQueue.RemoveAt(0);
		}
		else if (FDateTime::UtcNow().ToUnixTimestamp() - PendingMessageQueue[0].TimeStamp >= 2000)
		{
			UE_LOG(LogTemp, Fatal, TEXT("Pending message acknowledgment expired. State of games is unreliable"))

			// Dequeue
			PendingMessageQueue.RemoveAt(0);
		}
	}

	// If the queue is still not empty after the potential deque above and the first message publishing is not requested, request publishing of the first message
	if (!PendingMessageQueue.IsEmpty() && !PendingMessageQueue[0].bPublishRequested)
	{
		const FString PayloadString = PendingMessageQueue[0].Payload;
		UE_LOG(LogTemp, Display, TEXT("ACTUALLY PUBLISHING MESSAGE: %s"), *PayloadString);
		//const uint32 size = PayloadString.Len();

		//TArray<uint8> PayloadBytes;
		//PayloadBytes.AddUninitialized(size);
		//StringToBytes(PayloadString, PayloadBytes.GetData(), size);

		//BPPublish(WallF1Config.TopicToPublishIn, TArray<uint8>((uint8*)TCHAR_TO_UTF8(*PayloadString), PayloadString.Len()), EMQTTQualityOfService::ExactlyOnce);

		MqttActor->Publish(WallF1Config.TopicToPublishIn, PayloadString, WallF1Config.QoS);

		PendingMessageQueue[0].bPublishRequested = true;
	}
}

void UWallF1SensorHandler::QueueMessage(const FString& Message)
{
	UE_LOG(LogTemp, Display, TEXT("REQUESTING MESSAGE PUBLISH: %s"), *Message);

	FWallF1PendingMessage PendingMessage;
	PendingMessage.Payload = Message;
	PendingMessage.bPublishRequested = false;
	PendingMessage.bAcknowledged = false;
	PendingMessage.TimeStamp = FDateTime::UtcNow().ToUnixTimestamp();
	PendingMessageQueue.Add(PendingMessage);
}

void UWallF1SensorHandler::OnMessageReceived()
{
	//FString receivedPayload = message.GetPayloadAsString();
	//UE_LOG(LogTemp, Display, TEXT("MESSAGE RECEIVED: %s"), *receivedPayload);

	//bool bIsACK = receivedPayload.Contains("ACK");
	//if (bIsACK)
	//	HandleACKReceived();
	//else
	//{
	//	// Parse json message
	//	FWallF1SensorResponse SensorResponse;
	//	FJsonObjectConverter::JsonObjectStringToUStruct(receivedPayload, &SensorResponse);

	//	// Broadcast delegate
	//	OnSensorDetection.Broadcast(SensorResponse.idSensor);
	//}
}

void UWallF1SensorHandler::OnConnected()
{
}

void UWallF1SensorHandler::HandleACKReceived()
{
	if (PendingMessageQueue.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Recived ACK but pending message queue is empty!"))
			return;
	}

	PendingMessageQueue[0].bAcknowledged = true;
}
