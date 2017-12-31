#pragma once

#include "cocos2d.h"

USING_NS_CC;

class ConfigManager
{
public:
	static ConfigManager* getInstance();

	const Size& getDesignResolution() const { return _designResolution; }

	int getNumBrickRows() const { return _numBrickRows; }
	int getNumBricksInRow() const { return _numBricksInRow; }
	const Vec2& getFirstBrickPos() const { return _firstBrickPos; }
	const Size& getBrickSize() const { return _brickSize; }

	const Vec2& getLeftBorderPos() const { return _leftBorderPos; }
	const Size& getLeftBorderSize() const { return _leftBorderSize; }

	const Vec2& getRightBorderPos() const { return _rightBorderPos; }
	const Size& getRightBorderSize() const { return _rightBorderSize; }

	const Vec2& getTopBorderPos() const { return _topBorderPos; }
	const Size& getTopBorderSize() const { return _topBorderSize; }

	const Vec2& getPaddlePos() const { return _paddlePos; }
	const Size& getPaddleSize() const { return _paddleSize; }

	const Vec2& getBallPos() const { return _ballPos; }
	const Size& getBallSize() const { return _ballSize; }

private:
	ConfigManager();
	ConfigManager(const ConfigManager&) = delete;
	ConfigManager& operator= (const ConfigManager&) = delete;
	~ConfigManager() = default;

private:
	Size _designResolution;

	int _numBrickRows;
	int _numBricksInRow;
	Vec2 _firstBrickPos;
	Size _brickSize;

	Vec2 _leftBorderPos;
	Size _leftBorderSize;

	Vec2 _rightBorderPos;
	Size _rightBorderSize;

	Vec2 _topBorderPos;
	Size _topBorderSize;

	Vec2 _paddlePos;
	Size _paddleSize;

	Vec2 _ballPos;
	Size _ballSize;
};