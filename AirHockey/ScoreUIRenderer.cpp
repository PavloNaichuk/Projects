#include "pch.h"
#include "ScoreUIRenderer.h"
#include "ScoreUILogic.h"
#include "BoxComponent.h"
#include "GameObject.h"
#include "ScoreUIData.h"

ScoreUIRenderer::ScoreUIRenderer(SharedRenderer renderer, SharedResourceManager resourceManager)
	: mRenderer(renderer)
	, mTextFont(resourceManager->GetTextFont(ResourceManager::SCORE_TEXT_ID))
	, mPrevPlayerScore(-1)
	, mPrevEnemyScore(-1)
{
}

Component::ComponentId ScoreUIRenderer::GetId() const
{
	return RenderComponent::COMPONENT_ID;
};

void ScoreUIRenderer::Render(GameObject& gameObject) 
{
	ScoreUIData* scoreUIData = gameObject.GetComponent<ScoreUIData>(ScoreUIData::COMPONENT_ID);
	BoxComponent* boxComponent = gameObject.GetComponent<BoxComponent>(BoxComponent::COMPONENT_ID);

	int currentPlayerScore = scoreUIData->GetPlayerScore();
	int currentEnemyScore = scoreUIData->GetEnemyScore();

	if ((mPrevPlayerScore != currentPlayerScore) || (mPrevEnemyScore != currentEnemyScore)) 
	{
		SDL_Color textColor = {255, 255, 255};

		char textBuffer[16];
		std::snprintf(textBuffer, sizeof(textBuffer), "%d:%d", currentEnemyScore, currentPlayerScore);
		
		mTextSurface.reset(TTF_RenderText_Solid(mTextFont.get(), textBuffer, textColor), SDL_FreeSurface);
		assert(mTextSurface);

		mTextTexture.reset(SDL_CreateTextureFromSurface(mRenderer.get(), mTextSurface.get()), SDL_DestroyTexture); 
		assert(mTextTexture);

		mPrevPlayerScore = currentPlayerScore;
		mPrevEnemyScore = currentEnemyScore;
	}
	
	int textWidth, textHeight;
	SDL_QueryTexture(mTextTexture.get(), nullptr, nullptr, &textWidth, &textHeight);

	assert(textWidth <= boxComponent->GetWidth());
	assert(textHeight <= boxComponent->GetHeight());

	SDL_Rect destRect = {
		int(boxComponent->GetTopLeft().mX) + (boxComponent->GetWidth() - textWidth) / 2, 
		int(boxComponent->GetTopLeft().mY) + (boxComponent->GetHeight() - textHeight) / 2,
		textWidth, 
		textHeight
	};
	SDL_RenderCopy(mRenderer.get(), mTextTexture.get(), nullptr, &destRect);
}