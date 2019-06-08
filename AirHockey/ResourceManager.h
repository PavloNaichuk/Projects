#pragma once

#include "Common.h"

class ResourceManager
{
public:
	enum ResourceId 
	{
		PlayerStriker,
		EnemyStriker,
		Puck,
		NumResources
	};

	bool LoadResources(SharedRendererPointer renderer);
	SharedTexturePointer GetTexture(ResourceId resourceId);

private:
	std::array<SharedSurfacePointer, NumResources> mSurfaces;
	std::array<SharedTexturePointer, NumResources> mTextures;
};

using SharedResourceManagerPointer = std::shared_ptr<ResourceManager>;