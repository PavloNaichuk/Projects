#pragma once

#include "cocos2d.h"
#include <string>

USING_NS_CC;

class GameFactory
{
public:
	static Sprite* createBrick(int lives, const Vec2& position, const Size& size);
	static Sprite* createLeftWall(const Vec2& position, const Size& size);
	static Sprite* createRightWall(const Vec2& position, const Size& size);
	static Sprite* createTopWall(const Vec2& position, const Size& size);
	static Sprite* createPaddle(const Vec2& position, const Size& size);
	static Sprite* createBall(const Vec2& position, float radius);
	static Node* createExitZone(const Vec2& start, const Vec2& end);

	static Label* createLabel(const Vec2& position, float fontSize, const std::string& text);
	static MenuItem* createMenuItem(const Vec2& position, float fontSize, const std::string& text);
	static Action* createMenuItemAnimation();
	static TransitionScene* createTransitionScene(Scene* scene);
};