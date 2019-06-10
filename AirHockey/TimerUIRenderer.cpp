#include "pch.h"
#include "TimerUIRenderer.h"
#include "TimerUILogic.h"
#include "BoxComponent.h"
#include "GameObject.h"

TimerUIRenderer::TimerUIRenderer(SharedRenderer renderer, SharedResourceManager resourceManager)
	: mRenderer(renderer)
	, mTextFont(resourceManager->GetTextFont(ResourceManager::TIMER_TEXT_ID))
	, mPrevTime(-1)
{
}

Component::ComponentId TimerUIRenderer::GetId() const
{
	return RenderComponent::COMPONENT_ID;
};

void TimerUIRenderer::Render(GameObject& gameObject)
{
	TimerUILogic* logic = gameObject.GetComponent<TimerUILogic>(TimerUILogic::COMPONENT_ID);
	BoxComponent* boxComponent = gameObject.GetComponent<BoxComponent>(BoxComponent::COMPONENT_ID);

	int currentTime = int(logic->GetTime());
	if (mPrevTime != currentTime) 
	{
		SDL_Color textColor = {255, 255, 255};
		
		char textBuffer[16];
		std::snprintf(textBuffer, sizeof(textBuffer), "Time: %d", currentTime);
		mPrevTime = currentTime;

		mTextSurface.reset(TTF_RenderText_Solid(mTextFont.get(), textBuffer, textColor), SDL_FreeSurface);
		assert(mTextSurface);
		
		mTextTexture.reset(SDL_CreateTextureFromSurface(mRenderer.get(), mTextSurface.get()), SDL_DestroyTexture);
		assert(mTextTexture);
	}

	int textWidth, textHeight;
	SDL_QueryTexture(mTextTexture.get(), nullptr, nullptr, &textWidth, &textHeight);

	assert(textWidth <= boxComponent->GetWidth());
	assert(textHeight <= boxComponent->GetHeight());

	SDL_Rect destRect = {int(boxComponent->GetTopLeft().mX), int(boxComponent->GetTopLeft().mY), textWidth, textHeight};
	SDL_RenderCopy(mRenderer.get(), mTextTexture.get(), nullptr, &destRect);
}