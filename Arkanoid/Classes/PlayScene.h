#pragma once

#include "cocos2d.h"

USING_NS_CC;

class GameController;

class PlayScene : public Scene
{
public:
	PlayScene(GameController* gameController);
	static PlayScene* create(GameController* gameController);
	
	bool init() override;

private:
	void updateScore(int score);

	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	
	bool onContactBegin(PhysicsContact& contact);
	bool onContactPostSolve(PhysicsContact& contact);
	bool onContactSeparate(PhysicsContact& contact);
	
private:
	GameController* _gameController = nullptr;
	Sprite* _paddle = nullptr;
	float _paddleSpeed = 0.0f;
	Sprite* _ball = nullptr;
	float _ballSpeed = 0.0f;
	Label* _scoreLabel = nullptr;
	int _score = 0;
	bool _allowUserInput = false;
	int _numDestroyedBricks = 0;
};
