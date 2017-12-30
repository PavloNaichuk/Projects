#include "ConfigManager.h"

ConfigManager::ConfigManager()
	: _numBrickRows(5)
	, _numBricksInRow(15)
	, _firstBrickPos(0, 500)
	, _brickSize(38, 19)
	, _leftBorderPos()
	, _leftBorderSize()
	, _rightBorderPos()
	, _rightBorderSize()
	, _topBorderPos()
	, _topBorderSize()
	, _paddlePos()
	, _paddleSize()
	, _ballPos()
	, _ballSize()
{}

ConfigManager* ConfigManager::getInstance()
{
	static ConfigManager instance;
	return &instance;
}
