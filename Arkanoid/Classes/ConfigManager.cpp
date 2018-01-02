#include "ConfigManager.h"

ConfigManager::ConfigManager()
	: _designResolution(512.0f, 540.0f)
	, _numBrickRows(5)
	, _numBricksInRow(15)
	, _firstBrickPos(32.0f, 440.0f)
	, _brickSize(32.0f, 16.0f)
	, _leftBorderPos(8.0f, 242.0f)
	, _leftBorderSize(16.0f, 484.0f)
	, _rightBorderPos(504.0f, 242.0f)
	, _rightBorderSize(16.0f, 484.0f)
	, _topBorderPos(256.0f, 492.0f)
	, _topBorderSize(512.0f, 16.0f)
	, _bottomBorderStart(0.0f, 0.0f)
	, _bottomBorderEnd(512.0f, 0.0f)
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
