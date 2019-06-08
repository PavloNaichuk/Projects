#pragma once

#include "Common.h"

class ResourceManager
{
public:
	enum ResourceId 
	{
		BoardId,
		PlayerStrikerId,
		EnemyStrikerId,
		PuckId,
		NumResources
	};

	bool LoadResources(SharedRenderer renderer);
	SharedTexture GetTexture(ResourceId resourceId);

private:
	std::array<SharedSurface, NumResources> mSurfaces;
	std::array<SharedTexture, NumResources> mTextures;
};

using SharedResourceManager = std::shared_ptr<ResourceManager>;