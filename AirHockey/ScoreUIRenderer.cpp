#include "pch.h"
#include "ScoreUIRenderer.h"
#include "ScoreUILogic.h"
#include "BoxComponent.h"
#include "GameObject.h"

ScoreUIRenderer::ScoreUIRenderer(SharedRenderer renderer, SharedResourceManager resourceManager)
	: mRenderer(renderer)
	, mTextFont(resourceManager->GetTextFont(ResourceManager::SCORE_TEXT_ID))
	, mPrevScore(-1)
{
}

Component::ComponentId ScoreUIRenderer::GetId() const
{
	return RenderComponent::COMPONENT_ID;
};

void ScoreUIRenderer::Render(GameObject& gameObject) 
{
	ScoreUILogic* playerStrikerLogic = gameObject.GetComponent<ScoreUILogic>(ScoreUILogic::COMPONENT_ID);
	ScoreUILogic* enemyStrikerLogic = gameObject.GetComponent<ScoreUILogic>(ScoreUILogic::COMPONENT_ID);

	BoxComponent* boxComponent = gameObject.GetComponent<BoxComponent>(BoxComponent::COMPONENT_ID);

	unsigned currentPlayerStrikerScore = int(playerStrikerLogic->GetScore());
	unsigned currentEnemyStrikerScore = int(enemyStrikerLogic->GetScore());
	
	SDL_Color textColor = { 255, 255, 255 };

	char textBuffer[16];
	std::snprintf(textBuffer, sizeof(textBuffer), "Score: %d - %d", currentPlayerStrikerScore, currentEnemyStrikerScore);

	mTextSurface.reset(TTF_RenderText_Solid(mTextFont.get(), textBuffer, textColor), SDL_FreeSurface);
	assert(mTextSurface);

	mTextTexture.reset(SDL_CreateTextureFromSurface(mRenderer.get(), mTextSurface.get()), SDL_DestroyTexture);
	assert(mTextTexture);

	int textWidth, textHeight;
	SDL_QueryTexture(mTextTexture.get(), nullptr, nullptr, &textWidth, &textHeight);

	assert(textWidth <= boxComponent->GetWidth());
	assert(textHeight <= boxComponent->GetHeight());

	SDL_Rect destRect = { int(boxComponent->GetTopLeft().mX), int(boxComponent->GetTopLeft().mY), textWidth, textHeight };
	SDL_RenderCopy(mRenderer.get(), mTextTexture.get(), nullptr, &destRect);
}