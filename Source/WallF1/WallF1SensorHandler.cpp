#include "WallF1SensorHandler.h"
//#include "MqttUtilitiesBPL.h"
//#include "MqttUtilities/Public/Entities/MqttClientConfig.h"
//#include "MqttUtilities/Public/Entities/MqttConnectionData.h"
//#include "MqttUtilities/Public/Entities/MqttMessage.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "MQTTClientObject.h"
#include "MQTTSubsystem.h"

FWallF1SensorColor UWallF1SensorHandler::DefaultDisplayColor = FWallF1SensorColor();

void UWallF1SensorHandler::Initialize(FWallF1Config InConfig)
{
	MqttClient = UMQTTSubsystem::GetOrCreateClient_WithProjectURL(this);

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
	//	UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
	//	return;
	//}

	//FMqttMessage message;
	//message.Topic = WallF1Config.TopicToPublishIn;
	//message.Message = FString::Printf(TEXT("{\"modo\":0,\"idSensor\":%i}"), SensorId);
	//QueueMessage(message);

	//SensorsState[SensorId - 1] = EWallF1SensorState::DETECTION_ENABLED;
}

void UWallF1SensorHandler::DisableSensorDetection(uint8 SensorId)
{
	//if (!MqttClient)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
	//	return;
	//}

	//FMqttMessage message;
	//message.Topic = WallF1Config.TopicToPublishIn;
	//message.Message = FString::Printf(TEXT("{\"modo\":1,\"idSensor\":%i}"), SensorId);
	//QueueMessage(message);

	//SensorsState[SensorId - 1] = EWallF1SensorState::OFF;
}

void UWallF1SensorHandler::TurnOnLed(uint8 SensorId, FWallF1SensorColor InColor)
{
	//if (!MqttClient)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
	//	return;
	//}

	//FMqttMessage message;
	//message.Topic = WallF1Config.TopicToPublishIn;
	//message.Message = FString::Printf(TEXT("{\"modo\":2,\"idSensor\":%i,\"r\":%i,\"g\":%i,\"b\":%i}"), SensorId, InColor.r, InColor.g, InColor.b);
	//QueueMessage(message);

	//SensorsState[SensorId - 1] = EWallF1SensorState::LED_ON;
}

void UWallF1SensorHandler::TurnOffLed(uint8 SensorId)
{
	//if (!MqttClient)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
	//	return;
	//}

	//FMqttMessage message;
	//message.Topic = WallF1Config.TopicToPublishIn;
	//message.Message = FString::Printf(TEXT("{\"modo\":2,\"idSensor\":%i,\"r\":0,\"g\":0,\"b\":0}"), SensorId);
	//QueueMessage(message);

	//SensorsState[SensorId - 1] = EWallF1SensorState::OFF;
}

void UWallF1SensorHandler::EnableAllSensorsDetection()
{
	//if (!MqttClient)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
	//	return;
	//}

	//FMqttMessage message;
	//message.Topic = WallF1Config.TopicToPublishIn;
	//message.Message = "{\"modo\":0,\"idSensor\":0}";
	//QueueMessage(message);

	//for (int i = 0; i < 9; ++i)
	//{
	//	SensorsState[i] = EWallF1SensorState::DETECTION_ENABLED;
	//}
}

void UWallF1SensorHandler::DisableAllSensorsDetection()
{
	//if (!MqttClient)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
	//	return;
	//}

	//FMqttMessage message;
	//message.Topic = WallF1Config.TopicToPublishIn;
	//message.Message = "{\"modo\":1,\"idSensor\":0}";
	//QueueMessage(message);

	//for (int i = 0; i < 9; ++i)
	//{
	//	SensorsState[i] = EWallF1SensorState::OFF;
	//}
}

void UWallF1SensorHandler::TurnOnAllLeds(FWallF1SensorColor InColor)
{
	//if (!MqttClient)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
	//	return;
	//}

	//FMqttMessage message;
	//message.Topic = WallF1Config.TopicToPublishIn;
	//message.Message = FString::Printf(TEXT("{\"modo\":2,\"idSensor\":0,\"r\":%i,\"g\":%i,\"b\":%i}"), InColor.r, InColor.g, InColor.b);
	//QueueMessage(message);

	//for (int i = 0; i < 9; ++i)
	//{
	//	SensorsState[i] = EWallF1SensorState::LED_ON;
	//}
}

void UWallF1SensorHandler::TurnOffAllLeds()
{
	//ensure()

	//if (!MqttClient)
	//{
		//UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
		//return;
	//}

	//FMqttMessage message;
	//message.Topic = WallF1Config.TopicToPublishIn;
	//message.Message = "{\"modo\":2,\"idSensor\":0,\"r\":0,\"g\":0,\"b\":0}";
	//QueueMessage(message);

	//for (int i = 0; i < 9; ++i)
	//{
	//	SensorsState[i] = EWallF1SensorState::OFF;
	//}
}

void UWallF1SensorHandler::SetDefaultDisplayColor(FWallF1SensorColor InColor)
{
	UWallF1SensorHandler::DefaultDisplayColor = InColor;
}

void UWallF1SensorHandler::SetDetectionColorOfAllSensors(FWallF1SensorColor InColor)
{
	//if (!MqttClient)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("MqttClient needs to be initialized initialized"));
	//	return;
	//}

	//FMqttMessage message;
	//message.Topic = WallF1Config.TopicToPublishIn;
	//message.Message = FString::Printf(TEXT("{\"modo\":3,\"idSensor\":0,\"r\":%i,\"g\":%i,\"b\":%i}"), InColor.r, InColor.g, InColor.b);
	//QueueMessage(message);
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
//	// Remove any acknowledged or expired message
//	if (!PendingMessageQueue.IsEmpty())
//	{
//		if(PendingMessageQueue[0].bAcknowledged)
//		{
//			if(!PendingMessageQueue[0].bPublishRequested)
//				UE_LOG(LogTemp, Fatal, TEXT("Not published pending message was acknowledged. State of games is inreliable"))
//
//			// Dequeue
//			PendingMessageQueue.RemoveAt(0);
//		}
//		else if (FDateTime::UtcNow().ToUnixTimestamp() - PendingMessageQueue[0].TimeStamp >= 2000)
//		{
//			UE_LOG(LogTemp, Fatal, TEXT("Pending message acknowledgment expired. State of games is inreliable"))
//
//			// Dequeue
//			PendingMessageQueue.RemoveAt(0);
//		}
//	}
//
//	// If the queue is still not empty after the potential message removal and the first message publishing is not requeted, request publishing of the first message
//	if (!PendingMessageQueue.IsEmpty() && !PendingMessageQueue[0].bPublishRequested)
//	{
//		//MqttClient->Publish(PendingMessageQueue[0].MqttMessage);
//		PendingMessageQueue[0].bPublishRequested = true;
//	}
}

bool UWallF1SensorHandler::IsTickable() const
{
	return true;
}

TStatId UWallF1SensorHandler::GetStatId() const
{
	return TStatId();
}

bool UWallF1SensorHandler::IsTickableInEditor() const
{
	return true;
}

void UWallF1SensorHandler::QueueMessage(FWallF1MqttMessage Message)
{
	//UE_LOG(LogTemp, Display, TEXT("REQUESTING MESSAGE PUBLISH: %s"), *Message.Message);
	//FWallF1PendingMessage PendingMessage;
	//PendingMessage.MqttMessage = Message;
	//PendingMessage.bPublishRequested = false;
	//PendingMessage.bAcknowledged = false;
	//PendingMessage.TimeStamp = FDateTime::UtcNow().ToUnixTimestamp();
	//PendingMessageQueue.Add(PendingMessage);
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

void UWallF1SensorHandler::OnMessageReceived(FWallF1MqttMessage message)
{
	//UE_LOG(LogTemp, Display, TEXT("MESSAGE RECEIVED: %s"), *message.Message);

	//bool bIsACK = message.Message.Contains("ACK");
	//if (bIsACK)
	//	HandleACKReceived(message);
	//else
	//{
	//	// Parse json message
	//	FWallF1SensorResponse SensorResponse;
	//	FJsonObjectConverter::JsonObjectStringToUStruct(message.Message, &SensorResponse);

	//	// Broadcast delegate
	//	OnSensorDetection.Broadcast(SensorResponse.idSensor);
	//}
}

void UWallF1SensorHandler::HandleACKReceived(FWallF1MqttMessage message)
{
	//if(PendingMessageQueue.IsEmpty())
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Recived ACK but pending message queue is empty!"))
	//	return;
	//}

	//PendingMessageQueue[0].bAcknowledged = true;
}
