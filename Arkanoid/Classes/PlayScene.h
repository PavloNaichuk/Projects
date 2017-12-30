#pragma once

#include "cocos2d.h"
#include "Utilities.h"

USING_NS_CC;

class PlayScene : public Ref
{
public:
	static Scene* create();

private:
	static Sprite* createBrick(BrickType brickType, const Vec2& position, const Size& size);
	static Sprite* createLeftBorder(const Vec2& position, const Size& size);
	static Sprite* createRightBorder(const Vec2& position, const Size& size);
	static Sprite* createTopBorder(const Vec2& position, const Size& size);
	static Sprite* createPaddle(const Vec2& position, const Size& size);
	static Sprite* createBall(const Vec2& position, const Size& size);
};