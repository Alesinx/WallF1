// Fill out your copyright notice in the Description page of Project Settings.


#include "WallF1SensorHandler.h"
#include "MqttUtilitiesBPL.h"
#include "MqttUtilities/Public/Entities/MqttClientConfig.h"
#include "MqttUtilities/Public/Entities/MqttConnectionData.h"
#include "MqttUtilities/Public/Entities/MqttMessage.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"

void UWallF1SensorHandler::Initialize()
{
	FMqttClientConfig config;
	config.HostUrl = "test.mosquitto.org";
	config.Port = 1883;
	config.ClientId = "User";
	config.EventLoopDeltaMs = -1;

	MqttClient = UMqttUtilitiesBPL::CreateMqttClient(config);

	if(!MqttClient)
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
	MqttClient->Subscribe("tikonos/visualizer", 0);

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
	message.Topic = "tikonos/detector";
	message.Message = FString::Printf(TEXT("{\"modo\":0,\"idSensor\":%i}"), SensorId);

	MqttClient->Publish(message);
}

void UWallF1SensorHandler::DisableSensorDetection(uint8 SensorId)
{
	if (!MqttClient)
	{
		UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
		return;
	}

	FMqttMessage message;
	message.Topic = "tikonos/detector";
	message.Message = FString::Printf(TEXT("{\"modo\":1,\"idSensor\":%i}"), SensorId);

	MqttClient->Publish(message);
}

void UWallF1SensorHandler::TurnOnLed(uint8 SensorId)
{
	if (!MqttClient)
	{
		UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
		return;
	}

	FMqttMessage message;
	message.Topic = "tikonos/detector";
	message.Message = FString::Printf(TEXT("{\"modo\":2,\"idSensor\":%i,\"r\":%i,\"g\":%i,\"b\":%i}"), SensorId, DisplayColor.r, DisplayColor.g, DisplayColor.b);

	MqttClient->Publish(message);
}

void UWallF1SensorHandler::TurnOffLed(uint8 SensorId)
{
	if (!MqttClient)
	{
		UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
		return;
	}

	FMqttMessage message;
	message.Topic = "tikonos/detector";
	message.Message = FString::Printf(TEXT("{\"modo\":2,\"idSensor\":%i,\"r\":0,\"g\":0,\"b\":0}"), SensorId);

	MqttClient->Publish(message);
}

void UWallF1SensorHandler::EnableAllSensorsDetection()
{
	if (!MqttClient)
	{
		UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
		return;
	}

	FMqttMessage message;
	message.Topic = "tikonos/detector";
	message.Message = "{\"modo\":0,\"idSensor\":0}";

	MqttClient->Publish(message);
}

void UWallF1SensorHandler::DisableAllSensorsDetection()
{
}

void UWallF1SensorHandler::TurnOnAllLeds()
{
	if (!MqttClient)
	{
		UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
		return;
	}

	FMqttMessage message;
	message.Topic = "tikonos/detector";
	message.Message = FString::Printf(TEXT("{\"modo\":2,\"idSensor\":0,\"r\":%i,\"g\":%i,\"b\":%i}"), DisplayColor.r, DisplayColor.g, DisplayColor.b);

	MqttClient->Publish(message);
}

void UWallF1SensorHandler::TurnOffAllLeds()
{
	if (!MqttClient)
	{
		UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
		return;
	}

	FMqttMessage message;
	message.Topic = "tikonos/detector";
	message.Message = "{\"modo\":2,\"idSensor\":0,\"r\":0,\"g\":0,\"b\":0}";

	MqttClient->Publish(message);
}

void UWallF1SensorHandler::SetDetectionColor(FWallF1SensorColor InColor)
{
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
	FWallF1SensorDetectionResponse SensorMessage;
	FJsonObjectConverter::JsonObjectStringToUStruct(message.Message, &SensorMessage);
	UE_LOG(LogTemp, Display, TEXT("MESSAGE RECEIVED: %s"), *message.Message);
	OnSensorDetection.Broadcast(SensorMessage.idSensor);
}