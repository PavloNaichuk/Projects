#pragma once

class GameState
{
public:
	virtual ~GameState() {}

	virtual void Enter() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
};