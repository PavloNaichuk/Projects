#pragma once

class Ghost;

class GhostAI
{
public:
	virtual ~GhostAI() {}
	virtual void Update(Ghost& ghost) = 0;
	virtual void Respawn(Ghost& ghost) = 0;
};