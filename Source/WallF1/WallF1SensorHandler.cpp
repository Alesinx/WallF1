#include "WallF1SensorHandler.h"
#include "MqttUtilitiesBPL.h"
#include "MqttUtilities/Public/Entities/MqttClientConfig.h"
#include "MqttUtilities/Public/Entities/MqttConnectionData.h"
#include "MqttUtilities/Public/Entities/MqttMessage.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"

void UWallF1SensorHandler::Initialize(FWallF1Config InConfig)
{
	WallF1Config = InConfig;
	FMqttClientConfig config;
	config.HostUrl = WallF1Config.Host;
	config.Port = WallF1Config.Port;
	config.ClientId = "User";
	config.EventLoopDeltaMs = -1;
	MqttClient = UMqttUtilitiesBPL::CreateMqttClient(config);
	if (!MqttClient)
	{
		UE_LOG(LogTemp, Error, TEXT("MqttClient could not be initialized"));
		return;
	}

	FMqttConnectionData ConnectionData;
	ConnectionData.Login = "";
	ConnectionData.Password = "";

	ConnectDelegate.BindUFunction(this, FName("OnClientConnected"));
	MqttClient->Connect(ConnectionData, ConnectDelegate);

	MessagePublishDelegate.BindUFunction(this, FName("OnMessagePublished"));
	MqttClient->SetOnPublishHandler(MessagePublishDelegate);

	SubscribeDelegate.BindUFunction(this, FName("OnSubscribed"));
	MqttClient->SetOnSubscribeHandler(SubscribeDelegate);
	MqttClient->Subscribe(WallF1Config.TopicToSubscribeTo, 0);

	MessageReceivedDelegate.BindUFunction(this, FName("OnMessageReceived"));
	MqttClient->SetOnMessageHandler(MessageReceivedDelegate);
}

void UWallF1SensorHandler::EnableSensorDetection(uint8 SensorId)
{
	if (!MqttClient)
	{
		UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
		return;
	}

	FMqttMessage message;
	message.Topic = WallF1Config.TopicToPublishIn;
	message.Message = FString::Printf(TEXT("{\"modo\":0,\"idSensor\":%i}"), SensorId);

	UE_LOG(LogTemp, Display, TEXT("PUBLISHING MESSAGE: %s"), *message.Message);
	MqttClient->Publish(message);

	SensorsState[SensorId - 1] = EWallF1SensorState::DETECTION_ENABLED;
}

void UWallF1SensorHandler::DisableSensorDetection(uint8 SensorId)
{
	if (!MqttClient)
	{
		UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
		return;
	}

	FMqttMessage message;
	message.Topic = WallF1Config.TopicToPublishIn;
	message.Message = FString::Printf(TEXT("{\"modo\":1,\"idSensor\":%i}"), SensorId);

	UE_LOG(LogTemp, Display, TEXT("PUBLISHING MESSAGE: %s"), *message.Message);
	MqttClient->Publish(message);

	SensorsState[SensorId - 1] = EWallF1SensorState::OFF;
}

void UWallF1SensorHandler::TurnOnLed(uint8 SensorId)
{
	if (!MqttClient)
	{
		UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
		return;
	}

	FMqttMessage message;
	message.Topic = WallF1Config.TopicToPublishIn;
	message.Message = FString::Printf(TEXT("{\"modo\":2,\"idSensor\":%i,\"r\":%i,\"g\":%i,\"b\":%i}"), SensorId, DisplayColor.r, DisplayColor.g, DisplayColor.b);

	MqttClient->Publish(message);

	SensorsState[SensorId - 1] = EWallF1SensorState::LED_ON;
}

void UWallF1SensorHandler::TurnOffLed(uint8 SensorId)
{
	if (!MqttClient)
	{
		UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
		return;
	}

	FMqttMessage message;
	message.Topic = WallF1Config.TopicToPublishIn;
	message.Message = FString::Printf(TEXT("{\"modo\":2,\"idSensor\":%i,\"r\":0,\"g\":0,\"b\":0}"), SensorId);

	UE_LOG(LogTemp, Display, TEXT("PUBLISHING MESSAGE: %s"), *message.Message);
	MqttClient->Publish(message);

	SensorsState[SensorId - 1] = EWallF1SensorState::OFF;
}

void UWallF1SensorHandler::EnableAllSensorsDetection()
{
	if (!MqttClient)
	{
		UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
		return;
	}

	FMqttMessage message;
	message.Topic = WallF1Config.TopicToPublishIn;
	message.Message = "{\"modo\":0,\"idSensor\":0}";

	UE_LOG(LogTemp, Display, TEXT("PUBLISHING MESSAGE: %s"), *message.Message);
	MqttClient->Publish(message);

	for (int i = 0; i < 9; ++i)
	{
		SensorsState[i] = EWallF1SensorState::DETECTION_ENABLED;
	}
}

void UWallF1SensorHandler::DisableAllSensorsDetection()
{
	if (!MqttClient)
	{
		UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
		return;
	}

	FMqttMessage message;
	message.Topic = WallF1Config.TopicToPublishIn;
	message.Message = "{\"modo\":1,\"idSensor\":0}";

	UE_LOG(LogTemp, Display, TEXT("PUBLISHING MESSAGE: %s"), *message.Message);
	MqttClient->Publish(message);

	for (int i = 0; i < 9; ++i)
	{
		SensorsState[i] = EWallF1SensorState::OFF;
	}
}

void UWallF1SensorHandler::TurnOnAllLeds()
{
	if (!MqttClient)
	{
		UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
		return;
	}

	FMqttMessage message;
	message.Topic = WallF1Config.TopicToPublishIn;
	message.Message = FString::Printf(TEXT("{\"modo\":2,\"idSensor\":0,\"r\":%i,\"g\":%i,\"b\":%i}"), DisplayColor.r, DisplayColor.g, DisplayColor.b);

	UE_LOG(LogTemp, Display, TEXT("PUBLISHING MESSAGE: %s"), *message.Message);
	MqttClient->Publish(message);

	for (int i = 0; i < 9; ++i)
	{
		SensorsState[i] = EWallF1SensorState::LED_ON;
	}
}

void UWallF1SensorHandler::TurnOffAllLeds()
{
	if (!MqttClient)
	{
		UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
		return;
	}

	FMqttMessage message;
	message.Topic = WallF1Config.TopicToPublishIn;
	message.Message = "{\"modo\":2,\"idSensor\":0,\"r\":0,\"g\":0,\"b\":0}";

	UE_LOG(LogTemp, Display, TEXT("PUBLISHING MESSAGE: %s"), *message.Message);
	MqttClient->Publish(message);

	for (int i = 0; i < 9; ++i)
	{
		SensorsState[i] = EWallF1SensorState::OFF;
	}
}

void UWallF1SensorHandler::SetDetectionColor(FWallF1SensorColor InColor)
{
	if (!MqttClient)
	{
		UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
		return;
	}

	FMqttMessage message;
	message.Topic = WallF1Config.TopicToPublishIn;
	message.Message = FString::Printf(TEXT("{\"modo\":3,\"idSensor\":0,\"r\":%i,\"g\":%i,\"b\":%i}"), DisplayColor.r, DisplayColor.g, DisplayColor.b);

	UE_LOG(LogTemp, Display, TEXT("PUBLISHING MESSAGE: %s"), *message.Message);
	MqttClient->Publish(message);
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

void UWallF1SensorHandler::OnClientConnected()
{
	UE_LOG(LogTemp, Display, TEXT("CLIENT CONNECTED"));
}

void UWallF1SensorHandler::OnMessagePublished(int mid)
{
	UE_LOG(LogTemp, Display, TEXT("MESSAGE PUBLISHED WITH MID: %i"), mid);
}

void UWallF1SensorHandler::OnSubscribed(int mid, const TArray<int>& qos)
{
	UE_LOG(LogTemp, Display, TEXT("CLIENT SUBSCRIBED"));
}

void UWallF1SensorHandler::OnMessageReceived(FMqttMessage message)
{
	UE_LOG(LogTemp, Display, TEXT("MESSAGE RECEIVED: %s"), *message.Message);

	bool bIsACK = message.Message.Contains("ACK");
	if (!bIsACK)
	{
		// Parse json message
		FWallF1SensorResponse SensorResponse;
		FJsonObjectConverter::JsonObjectStringToUStruct(message.Message, &SensorResponse);

		// Broadcast delegate
		OnSensorDetection.Broadcast(SensorResponse.idSensor);
	}
}
