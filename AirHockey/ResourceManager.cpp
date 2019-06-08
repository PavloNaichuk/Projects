#include "pch.h"
#include "ResourceManager.h"

bool ResourceManager::LoadResources(SharedRendererPointer renderer)
{
	std::array<const char*, NumResources> resourcePaths;
	resourcePaths[PlayerStriker] = "Resource/Images/player.png";
	resourcePaths[EnemyStriker] = "Resource/Images/enemy.png";
	resourcePaths[Puck] = "Resource/Images/puck.png";
	
	for (unsigned index = 0; index < resourcePaths.size(); ++index) 
	{
		mSurfaces[index].reset(IMG_Load(resourcePaths[index]), SDL_FreeSurface);
		if (mSurfaces[index] == nullptr) 
		{
			SDL_Log("IMG_Load error: %s", IMG_GetError());
			return false;
		}
		
		mTextures[index].reset(SDL_CreateTextureFromSurface(renderer.get(), mSurfaces[index].get()), SDL_DestroyTexture);
		if (mTextures[index] == nullptr) 
		{
			SDL_Log("Unable to create texture: %s", SDL_GetError());
			return false;
		}
	}

	return true;
}

SharedTexturePointer ResourceManager::GetTexture(ResourceId resourceId) 
{
	return mTextures[resourceId];
}