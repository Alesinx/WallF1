#include "WallF1GameModeInGameTopScore.h"
#include "../WallF1GameInstance.h"

AWallF1GameModeInGameTopScore::AWallF1GameModeInGameTopScore() : Super()
{
	WallF1GameMode = EWallF1GameMode::TOP_SCORE;
}

void AWallF1GameModeInGameTopScore::StartWallF1Game()
{
}

void AWallF1GameModeInGameTopScore::HandleSensorDetection(int SensorId)
{
}