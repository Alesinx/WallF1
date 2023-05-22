/*
*  Copyright (c) 2020-2020 Damody(t1238142000@gmail.com).
*  All rights reserved.
*  @ Date : 2020/08/06
*
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MQTTAsync.h"
#include <string>
#include <sstream>
#include <chrono>
#include <vector>
#include "MqttActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageReceivedNative, FString, Payload);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnectedFailed);

UENUM(BlueprintType)
enum class EMqttCode : uint8
{
	MQTT_SUCCESS,
	MQTT_FAIL,
	MQTT_DISCONNECTING,
	MQTT_NON_CREATE,
};

UENUM(BlueprintType)
enum class EMqttConnection : uint8
{
	MQTT_TCP,
	MQTT_SSL,
	MQTT_WS,
	MQTT_WSS,
};

typedef std::chrono::system_clock::time_point TimePoint;

UCLASS()
class BPMQTT_API UMqttActor : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UMqttActor();
	~UMqttActor();
	struct MqttMsg {
		void* context;
		std::string topic;
		TArray<uint8> msg;
	};
	struct FailData {
		void* ptr;
		FString msg;
	};

	UPROPERTY()
	FOnMessageReceivedNative OnMessageReceivedNativeDelegate;

	UPROPERTY()
	FOnConnected OnConnectedDelegate;

	UPROPERTY()
	FOnConnectedFailed OnConnectedFailedDelegate;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return true; };
	virtual TStatId GetStatId() const override { return TStatId(); }
	virtual bool IsTickableInEditor() const override { return true; }

	UFUNCTION(BlueprintCallable, Category = "mqtt")
	EMqttCode CreateClient(FString In_sClientID, FString In_sTcpAddress, int32 In_iPort = 1883, EMqttConnection In_connection = EMqttConnection::MQTT_TCP);

	UFUNCTION(BlueprintCallable, Category = "mqtt")
	EMqttCode DestroyClient();

	float count_time = 0;

	UFUNCTION(BlueprintCallable, Category = "mqtt")
	EMqttCode ConnectSSL(int32 In_iTimeoutSec, bool In_bClean, FString In_username, FString In_password, FString trustStore, FString keyStore, FString privateKey, FString privateKeyPassword, FString enabledCipherSuites, FString CApath, int32 enableServerCertAuth, int32 verify);
	
	UFUNCTION(BlueprintCallable, Category = "mqtt")
	EMqttCode Connect(int32 In_iTimeoutSec = 5, bool In_bClean = true, FString In_username = FString(""), FString In_password = FString(""));

	UFUNCTION(BlueprintCallable, Category = "mqtt")
	EMqttCode Disconnect();

	UFUNCTION(BlueprintCallable, Category = "mqtt")
	EMqttCode IsConnected();

	UFUNCTION(BlueprintCallable, Category = "mqtt")
	EMqttCode Subscribe(FString In_sTopic, int32 In_iQOS = 0);

	UFUNCTION(BlueprintCallable, Category = "mqtt")
	EMqttCode UnSubscribe(FString In_sTopic);

	UFUNCTION(BlueprintCallable, Category = "mqtt")
	EMqttCode Publish(FString In_sTopic, FString In_sMsg, int32 In_iQOS = 0);

	UFUNCTION(BlueprintCallable, Category = "mqtt")
	EMqttCode PublishBytes(FString In_sTopic, const TArray<uint8>& In_sMsg, int32 In_iQOS = 0);

	UFUNCTION(BlueprintCallable, Category = "mqtt")
	EMqttCode PublishObject(FString In_sTopic, UObject* In_sMsg, int32 In_iQOS = 0);

	UFUNCTION(BlueprintImplementableEvent, Category = "mqtt")
	void OnReceive(const FString& sTopic, const TArray<uint8>& sMsg);

	UFUNCTION(BlueprintCallable, Category = "mqtt")
	static UObject* ConvertBytesToUObject(TArray<uint8> Data, UClass* TargetClass);

	UFUNCTION(BlueprintCallable, Category = "mqtt")
	static AActor* ConvertBytesToActor(TArray<uint8> Data, AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "mqtt")
	static TArray<uint8> ConvertUObjectToBytes(UObject* Target);

	UFUNCTION(BlueprintCallable, Category = "mqtt")
	static FString ConvertUObjectToJsonString(UObject* Target);

	UFUNCTION(BlueprintCallable, Category = "mqtt")
	static UObject* ConvertJsonStringToUObject(FString Target, UClass* TargetClass);

	UFUNCTION(BlueprintCallable, Category = "mqtt")
	static AActor* ConvertJsonStringToActor(FString Target, AActor* TargetActor);

	UFUNCTION(BlueprintPure, Category = "mqtt")
	static FString ConvertToString(TArray<uint8> Data);

	virtual void OnReceiveNative(const FString& sTopic, const TArray<uint8>& sMsg);

	virtual void OnConnectedNative();

	virtual void OnConnectedFailedNative();

	UFUNCTION(BlueprintImplementableEvent, Category = "mqtt")
	void OnConnected();

	UFUNCTION(BlueprintImplementableEvent, Category = "mqtt")
	void OnConnectedFail(const FString& FailMsg);


protected:
	EMqttCode PublishRaw(FString In_sTopic, char* In_pBuf, size_t In_BufSize, int32 In_iQOS = 0);

private:
	EMqttCode Start_Consuming();

	EMqttCode Stop_Consuming();

	static int on_message_arrived(void* context, char* topicName, int topicLen, MQTTAsync_message* msg);
	static void onfailure(void* context, MQTTAsync_failureData* response);
	static std::vector<MqttMsg> msgs;
	static std::vector<FailData> fails;

	EMqttCode Try_consume_message(FString& Out_sTopic, TArray<uint8>& Out_sMsg);

	bool m_bmqtt = false;
	bool m_bmqtt2;
	bool m_bInit = false;
	bool m_bDoReconnect = false;
	int32 m_iConnectTimeout;
	bool m_bCleanSession;
	TimePoint m_tpConnect;
	MQTTAsync client = nullptr;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
	MQTTAsync_responseOptions res_opts = MQTTAsync_responseOptions_initializer;
	MQTTAsync_message msg = MQTTAsync_message_initializer;
	int rc;
};
