#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WallF1GameInstance.h"
#include "MQTTShared.h"
#include "WallF1SensorHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSensorDetection, int, SensorId);

class UMQTTClientObject;
struct FMQTTClientMessage;

UENUM(BlueprintType)
enum class EWallF1SensorState : uint8
{
	OFF = 0,
	DETECTION_ENABLED = 1,
	LED_ON = 2
};

UENUM(BlueprintType)
enum class EWallF1SensorMode : uint8
{
	ENABLE_SENSOR = 0,
	DISABLE_SENSOR = 1,
	DISPLAY_COLOR = 2,
	SET_DETECTION_COLOR = 3,
};

//USTRUCT()
//struct WALLF1_API FWallF1MqttMessage
//{
//	GENERATED_BODY()
//
//	UPROPERTY()
//	FString payload;
//};

USTRUCT()
struct WALLF1_API FWallF1PendingMessage
{
	GENERATED_BODY()

	FString Payload;
	int64 TimeStamp = 0;
	bool bPublishRequested = false;
	bool bAcknowledged = false;
};

USTRUCT()
struct WALLF1_API FWallF1SensorColor
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 r = 255;

	UPROPERTY()
	uint8 g = 255;

	UPROPERTY()
	uint8 b = 255;
};

USTRUCT()
struct WALLF1_API FWallF1SensorResponse
{
	GENERATED_BODY()

	UPROPERTY()
	int modo;

	UPROPERTY()
	int idSensor;
};


/**
 * Class to handle WallF1 sensors
 */
UCLASS(Blueprintable)
class WALLF1_API UWallF1SensorHandler : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "WallF1 MQTT")
	void Initialize(FWallF1Config InConfig);

	void EnableSensorDetection(uint8 SensorId);
	void DisableSensorDetection(uint8 SensorId);
	void TurnOnLed(uint8 SensorId, FWallF1SensorColor InColor = DefaultDisplayColor);
	void TurnOffLed(uint8 SensorId);

	void EnableAllSensorsDetection();
	void DisableAllSensorsDetection();
	void TurnOnAllLeds(FWallF1SensorColor InColor);
	void TurnOffAllLeds();

	void SetDefaultDisplayColor(FWallF1SensorColor InColor);

	void SetDetectionColor(uint8 SensorId, FWallF1SensorColor InColor);
	void SetDetectionColorOfAllSensors(FWallF1SensorColor InColor);

	bool AreAllSensorsOff();

	UPROPERTY()
	FOnSensorDetection OnSensorDetection;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "WallF1 MQTT")
	UMQTTClientObject* MqttClient;

	UPROPERTY(BlueprintReadWrite, Category = "WallF1 MQTT")
	FWallF1Config WallF1Config;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "WallF1 MQTT")
	void BPConnect();

	UFUNCTION(BlueprintImplementableEvent, Category = "WallF1 MQTT")
	void BPSubscribe(UPARAM(DisplayName = "Topic") const FString& InTopic,
		UPARAM(DisplayName = "Quality of Service") EMQTTQualityOfService InQoS = EMQTTQualityOfService::ExactlyOnce);

	UFUNCTION(BlueprintImplementableEvent, Category = "WallF1 MQTT")
	void BPPublish(UPARAM(DisplayName = "Topic") const FString& InTopic,
		UPARAM(DisplayName = "Payload") const TArray<uint8>& InPayload,
		UPARAM(DisplayName = "Quality of Service") EMQTTQualityOfService InQoS = EMQTTQualityOfService::ExactlyOnce,
		const bool bInRetain = false);

	UFUNCTION(BlueprintCallable, Category = "WallF1 MQTT")
	void OnMessageReceived(const FMQTTClientMessage& message);

	UFUNCTION(BlueprintCallable, Category = "WallF1 MQTT")
	void OnConnected();

private:
	UPROPERTY()
	EWallF1SensorState SensorsState[9];

	UPROPERTY()
	TArray<FWallF1PendingMessage> PendingMessageQueue;

	static FWallF1SensorColor DefaultDisplayColor;

	//FOnConnectDelegate ConnectDelegate;
	//FOnPublishDelegate MessagePublishDelegate;
	//FOnSubscribeDelegate SubscribeDelegate;
	//FOnMessageDelegate MessageReceivedDelegate;

	void Tick(float DeltaTime);
	virtual bool IsTickable() const override { return true; };
	virtual TStatId GetStatId() const override { return TStatId(); }
	virtual bool IsTickableInEditor() const override { return true; }

	UFUNCTION()
	void QueueMessage(const FString& Message);

	//UFUNCTION()
	//void OnClientConnected();

	//UFUNCTION()
	//void OnMessagePublished(int mid);

	//UFUNCTION()
	//void OnSubscribed(int mid, const TArray<int>& qos);	

	UFUNCTION()
	void HandleACKReceived();
};
