#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MqttUtilities/Public/Interface/MqttClientInterface.h"
#include "WallF1GameInstance.h"
#include "WallF1SensorHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSensorDetection, int, SensorId);

class UMqttClientInterface;
struct FMqttMessage;

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

USTRUCT()
struct WALLF1_API FWallF1PendingMessage
{
	GENERATED_BODY()

	FMqttMessage MqttMessage;
	int64 TimeStamp = 0;
	bool bPublishRequested = false;
	bool bAcknowledged = false;
};

/**
 * Class to handle WallF1 sensors
 */
UCLASS()
class WALLF1_API UWallF1SensorHandler : public UObject
{
	GENERATED_BODY()

public:
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

private:
	UPROPERTY()
	TScriptInterface<IMqttClientInterface> MqttClient;

	UPROPERTY()
	EWallF1SensorState SensorsState[9];

	UPROPERTY()
	TArray<FWallF1PendingMessage> PendingMessageQueue;

	static FWallF1SensorColor DefaultDisplayColor;

	FOnConnectDelegate ConnectDelegate;
	FOnPublishDelegate MessagePublishDelegate;
	FOnSubscribeDelegate SubscribeDelegate;
	FOnMessageDelegate MessageReceivedDelegate;

	UFUNCTION()
	void Tick(float DeltaTime);

	UFUNCTION()
	void QueueMessage(FMqttMessage Message);

	UFUNCTION()
	void OnClientConnected();

	UFUNCTION()
	void OnMessagePublished(int mid);

	UFUNCTION()
	void OnSubscribed(int mid, const TArray<int>& qos);
	
	UFUNCTION()
	void OnMessageReceived(FMqttMessage message);

	UFUNCTION()
	void HandleACKReceived(FMqttMessage message);

	FWallF1Config WallF1Config;
};
