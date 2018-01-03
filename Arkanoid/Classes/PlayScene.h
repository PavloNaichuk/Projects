#pragma once

#include "cocos2d.h"

USING_NS_CC;

class PlayScene : public Scene
{
public:
	CREATE_FUNC(PlayScene);
	bool init() override;

private:
	static Sprite* createBrick(int lives, const Vec2& position, const Size& size);
	static Sprite* createLeftWall(const Vec2& position, const Size& size);
	static Sprite* createRightWall(const Vec2& position, const Size& size);
	static Sprite* createTopWall(const Vec2& position, const Size& size);
	static Sprite* createPaddle(const Vec2& position, const Size& size);
	static Sprite* createBall(const Vec2& position, float radius);
	static Node* createExitZone(const Vec2& start, const Vec2& end);

	void updateScore(int score);

	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	bool onContactBegin(PhysicsContact& contact);

private:
	Sprite* _paddle = nullptr;
	float _paddleSpeed = 250.0f;
	Sprite* _ball = nullptr;
	float _ballSpeed = 200.0f;
	Label* _scoreLabel = nullptr;
	int _score = 0;
	bool _allowUserInput = false;
	int _numBricks = 0;
};
