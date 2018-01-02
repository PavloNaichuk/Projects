#include "ConfigManager.h"

ConfigManager::ConfigManager()
	: _worldSize(512.0f, 540.0f)
	, _numBrickRows(5)
	, _numBricksInRow(15)
	, _firstBrickPos(32.0f, 440.0f)
	, _brickSize(32.0f, 16.0f)
	, _leftWallPos(8.0f, 242.0f)
	, _leftWallSize(16.0f, 484.0f)
	, _rightWallPos(504.0f, 242.0f)
	, _rightWallSize(16.0f, 484.0f)
	, _topWallPos(256.0f, 492.0f)
	, _topWallSize(512.0f, 16.0f)
	, _exitZoneStart(0.0f, 0.0f)
	, _exitZoneEnd(512.0f, 0.0f)
	, _paddlePos(256.0f, 100.0f)
	, _paddleSize(78.0f, 16.0f)
	, _ballPos(256.0f, 116.0f)
	, _ballRadius(8.0f)
{}

ConfigManager* ConfigManager::getInstance()
{
	static ConfigManager instance;
	return &instance;
}
