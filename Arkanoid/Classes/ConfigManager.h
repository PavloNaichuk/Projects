#pragma once

#include "cocos2d.h"

USING_NS_CC;

class ConfigManager
{
public:
	static ConfigManager* getInstance();

	const Size& getWorldSize() const { return _worldSize; }

	int getMaxBrickLives() const { return _maxBrickLives; }
	int getNumBrickRows() const { return _numBrickRows; }
	int getNumBricksInRow() const { return _numBricksInRow; }
	const Vec2& getFirstBrickPos() const { return _firstBrickPos; }
	const Size& getBrickSize() const { return _brickSize; }

	const Vec2& getLeftWallPos() const { return _leftWallPos; }
	const Size& getLeftWallSize() const { return _leftWallSize; }

	const Vec2& getRightWallPos() const { return _rightWallPos; }
	const Size& getRightWallSize() const { return _rightWallSize; }

	const Vec2& getTopWallPos() const { return _topWallPos; }
	const Size& getTopWallSize() const { return _topWallSize; }

	const Vec2& getExitZoneStart() const { return _exitZoneStart; }
	const Vec2& getExitZoneEnd() const { return _exitZoneEnd; }

	float getPaddleSpeed() const { return _paddleSpeed; }
	const Vec2& getPaddlePos() const { return _paddlePos; }
	const Size& getPaddleSize() const { return _paddleSize; }

	float getBallSpeed() const { return _ballSpeed; }
	float getBallSpeedPercentChange() const { return _ballSpeedPercentChange; }
	const Vec2& getBallPos() const { return _ballPos; }
	float getBallRadius() const { return _ballRadius; }

	const Vec2& getScoreLabelPos() const { return _scoreLabelPos; }
	int getScore() const { return _score; }

private:
	ConfigManager();
	ConfigManager(const ConfigManager&) = delete;
	ConfigManager& operator= (const ConfigManager&) = delete;
	~ConfigManager() = default;

private:
	Size _worldSize;

	int _maxBrickLives;
	int _numBrickRows;
	int _numBricksInRow;
	Vec2 _firstBrickPos;
	Size _brickSize;

	Vec2 _leftWallPos;
	Size _leftWallSize;

	Vec2 _rightWallPos;
	Size _rightWallSize;

	Vec2 _topWallPos;
	Size _topWallSize;

	Vec2 _exitZoneStart;
	Vec2 _exitZoneEnd;

	float _paddleSpeed;
	Vec2 _paddlePos;
	Size _paddleSize;

	float _ballSpeed;
	float _ballSpeedPercentChange;
	Vec2 _ballPos;
	float _ballRadius;

	Vec2 _scoreLabelPos;
	int _score;
};