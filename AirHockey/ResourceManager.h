#pragma once

#include "Common.h"

class ResourceManager
{
public:
	enum ResourceId 
	{
		BOARD_ID,
		PLAYER_STRIKER_ID,
		ENEMY_STRIKER_ID,
		PUCK_ID,
		NUM_RESOURCES
	};

	bool LoadResources(SharedRenderer renderer);
	SharedTexture GetTexture(ResourceId resourceId);

private:
	std::array<SharedSurface, NUM_RESOURCES> mSurfaces;
	std::array<SharedTexture, NUM_RESOURCES> mTextures;
};

using SharedResourceManager = std::shared_ptr<ResourceManager>;