#include "pch.h"
#include "ResourceManager.h"

bool ResourceManager::LoadResources(SharedRenderer renderer)
{
	std::array<const char*, NUM_RESOURCES> resourcePaths;
	resourcePaths[BOARD_ID] = "Resources/Images/board.png";
	resourcePaths[PLAYER_STRIKER_ID] = "Resources/Images/player.png";
	resourcePaths[ENEMY_STRIKER_ID] = "Resources/Images/enemy.png";
	resourcePaths[PUCK_ID] = "Resources/Images/puck.png";
	
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
			assert(false);
			SDL_Log("Unable to create texture: %s", SDL_GetError());
			return false;
		}
	}

	std::array<int, NUM_TEXT_FONTS> textFontSizes;
	textFontSizes[SCORE_TEXT_ID] = 25;
	textFontSizes[TIMER_TEXT_ID] = 25;
	textFontSizes[GAME_TITLE_TEXT_ID] = 60;
	textFontSizes[NAVIGATION_TEXT_ID] = 25;
	textFontSizes[START_GAME_TEXT_ID] = 25;
	textFontSizes[GAME_OVER_TEXT_ID] = 60;

	for (unsigned index = 0; index < textFontSizes.size(); ++index) 
	{
		mTextFonts[index].reset(TTF_OpenFont("Resources/Fonts/Arial.ttf", textFontSizes[index]), TTF_CloseFont);
		if (mTextFonts[index] == nullptr) 
		{
			assert(false);
			SDL_Log("Unable to open font: %s", TTF_GetError());
			return false;
		}
	}

	return true;
}

SharedTexture ResourceManager::GetTexture(ResourceId resourceId) 
{
	return mTextures[resourceId];
}

SharedFont ResourceManager::GetTextFont(TextFontId textFontId) 
{
	return mTextFonts[textFontId];
}