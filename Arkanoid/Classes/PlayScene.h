#pragma once

#include "cocos2d.h"
#include "Utilities.h"

USING_NS_CC;

class PlayScene : public Scene
{
public:
	CREATE_FUNC(PlayScene);
	bool init() override;
	void update(float deltaTime) override;

private:
	static Sprite* createBrick(EntityType brickType, const Vec2& position, const Size& size);
	static Sprite* createLeftBorder(const Vec2& position, const Size& size);
	static Sprite* createRightBorder(const Vec2& position, const Size& size);
	static Sprite* createTopBorder(const Vec2& position, const Size& size);
	static Node* createBottomBorder(const Vec2& start, const Vec2& end);
	static Sprite* createPaddle(const Vec2& position, const Size& size);
	static Sprite* createBall(const Vec2& position, float radius);

	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	bool onContactBegin(PhysicsContact& contact);

private:
	Sprite* _paddle = nullptr;
	float _paddleSpeed = 250.0f;
	Sprite* _ball = nullptr;
	float _ballSpeed = 200.0f;
};
