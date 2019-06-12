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

	enum TextFontId 
	{
		SCORE_TEXT_ID,
		TIMER_TEXT_ID,
		GAME_OVER_ID,
		NUM_TEXT_FONTS
	};

	bool LoadResources(SharedRenderer renderer);

	SharedTexture GetTexture(ResourceId resourceId);
	SharedFont GetTextFont(TextFontId textFontId);

private:
	std::array<SharedSurface, NUM_RESOURCES> mSurfaces;
	std::array<SharedTexture, NUM_RESOURCES> mTextures;

	std::array<SharedFont, NUM_TEXT_FONTS> mTextFonts;
};

using SharedResourceManager = std::shared_ptr<ResourceManager>;