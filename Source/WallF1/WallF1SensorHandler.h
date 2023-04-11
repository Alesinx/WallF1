#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MqttUtilities/Public/Interface/MqttClientInterface.h"
#include "WallF1GameInstance.h"
#include "WallF1SensorHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSensorDetection, int, SensorId);

class UMqttClientInterface;

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
	void TurnOnLed(uint8 SensorId);
	void TurnOffLed(uint8 SensorId);

	void EnableAllSensorsDetection();
	void DisableAllSensorsDetection();
	void TurnOnAllLeds();
	void TurnOffAllLeds();

	void SetDisplayColor(FWallF1SensorColor InColor) { DisplayColor = InColor; }
	void SetDetectionColor(FWallF1SensorColor InColor);

	bool AreAllSensorsOff();

	UPROPERTY()
	FOnSensorDetection OnSensorDetection;

private:
	UPROPERTY()
	TScriptInterface<IMqttClientInterface> MqttClient;

	UPROPERTY()
	EWallF1SensorState SensorsState[9];

	FWallF1SensorColor DisplayColor;

	FOnConnectDelegate ConnectDelegate;
	FOnPublishDelegate MessagePublishDelegate;
	FOnSubscribeDelegate SubscribeDelegate;
	FOnMessageDelegate MessageReceivedDelegate;

	UFUNCTION()
	void OnClientConnected();

	UFUNCTION()
	void OnMessagePublished(int mid);	

	UFUNCTION()
	void OnSubscribed(int mid, const TArray<int>& qos);
	
	UFUNCTION()
	void OnMessageReceived(FMqttMessage message);

	FWallF1Config WallF1Config;
};
