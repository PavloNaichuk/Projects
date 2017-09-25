#include "PlayGameState.h"

PlayGameState::PlayGameState(PlayGameStateListener& listener, SDLRendererPointer renderer)
	: mListener(listener)
	, mTextColor({ 255, 255, 255, 255 })
	, mMarioTextFont(TTF_OpenFont("Resources/Fonts/Arial.TTF", 50), TTF_CloseFont)
	, mMarioTextSurface(TTF_RenderText_Solid(mMarioTextFont.get(), "Mario: 0", mTextColor), SDL_FreeSurface)
	, mMarioTextTexture(SDL_CreateTextureFromSurface(renderer.get(), mMarioTextSurface.get()), SDL_DestroyTexture)
	, mTimeTextFont(TTF_OpenFont("Resources/Fonts/Arial.TTF", 50), TTF_CloseFont)
	, mTimeTextSurface(TTF_RenderText_Solid(mTimeTextFont.get(), "Timer: 0", mTextColor), SDL_FreeSurface)
	, mTimeTextTexture(SDL_CreateTextureFromSurface(renderer.get(), mTimeTextSurface.get()), SDL_DestroyTexture)
	, mGameRenderer(renderer)
{
	mGameWorld.mMario.mCenter.mX = WINDOW_WIDTH / 2;
	mGameWorld.mMario.mCenter.mY = WINDOW_HEIGHT / 2;

	mGameWorld.mEnemies.emplace_back(State::Moving, Point(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
		Vector(-ENEMY_SPEED, 0.0f), Size(ENEMY_WIDTH, ENEMY_HEIGHT), ENEMY_SCORE);

	mGameWorld.mEnemies.emplace_back(State::Moving, Point(WINDOW_WIDTH / 3, WINDOW_HEIGHT / 3),
		Vector(-ENEMY_SPEED, 0.0f), Size(ENEMY_WIDTH, ENEMY_HEIGHT), ENEMY_SCORE);

	mGameWorld.mEnemies.emplace_back(State::Moving, Point(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4),
		Vector(-ENEMY_SPEED, 0.0f), Size(ENEMY_WIDTH, ENEMY_HEIGHT), ENEMY_SCORE);

	mGameWorld.mEnemies.emplace_back(State::Moving, Point(WINDOW_WIDTH / 5, WINDOW_HEIGHT / 5),
		Vector(-ENEMY_SPEED, 0.0f), Size(ENEMY_WIDTH, ENEMY_HEIGHT), ENEMY_SCORE);

	
	mGameWorld.mGoldCoins.emplace_back(State::Standing, Point(300.0f, 200.0f), Vector(0.0f, 0.0f),
		Size(GOLDCOIN_WIDTH, GOLDCOIN_HEIGHT), GOLDCOIN_SCORE);

	mGameWorld.mGoldCoins.emplace_back(State::Standing, Point(400.0f, 200.0f), Vector(0.0f, 0.0f),
		Size(GOLDCOIN_WIDTH, GOLDCOIN_HEIGHT), GOLDCOIN_SCORE);

	mGameWorld.mGoldCoins.emplace_back(State::Standing, Point(500.0f, 200.0f), Vector(0.0f, 0.0f),
		Size(GOLDCOIN_WIDTH, GOLDCOIN_HEIGHT), GOLDCOIN_SCORE);

	mGameWorld.mGoldCoins.emplace_back(State::Standing, Point(300.0f, 400.0f), Vector(0.0f, 0.0f),
		Size(GOLDCOIN_WIDTH, GOLDCOIN_HEIGHT), GOLDCOIN_SCORE);

	mGameWorld.mGoldCoins.emplace_back(State::Standing, Point(400.0f, 400.0f), Vector(0.0f, 0.0f),
		Size(GOLDCOIN_WIDTH, GOLDCOIN_HEIGHT), GOLDCOIN_SCORE);

	mGameWorld.mGoldCoins.emplace_back(State::Standing, Point(500.0f, 400.0f), Vector(0.0f, 0.0f), 
		Size(GOLDCOIN_WIDTH, GOLDCOIN_HEIGHT), GOLDCOIN_SCORE);

	mGameWorld.mPowerUpToLevel2.emplace_back(State::Standing, Point(800.0f, 300.0f), Vector(0.0f, 0.0f),
		Size(POWER_UP_TO_LEVEL2_WIDTH, POWER_UP_TO_LEVEL2_HEIGHT), POWER_UP_TO_LEVEL2_SCORE);

	mGameWorld.mPowerUpToLevel3.emplace_back(State::Standing, Point(100.0f, 400.0f), Vector(0.0f, 0.0f),
		Size(POWER_UP_TO_LEVEL3_WIDTH, POWER_UP_TO_LEVEL3_HEIGHT), POWER_UP_TO_LEVEL3_SCORE);
	

	if (mMarioTextFont == nullptr)
	{
		SDL_Log("Unable to create font: %s", TTF_GetError());
	}
	if (mMarioTextSurface == nullptr)
	{
		SDL_Log("Unable to create surface: %s", TTF_GetError());
	}
	if (mMarioTextTexture == nullptr)
	{
		SDL_Log("Unable to create texture: %s", SDL_GetError());
	}

	SDL_QueryTexture(mMarioTextTexture.get(), nullptr, nullptr, &mMarioTextRect.w, &mMarioTextRect.h);
	mMarioTextRect.x = 10;
	mMarioTextRect.y = 10;

	if (mTimeTextFont == nullptr)
	{
		SDL_Log("Unable to create font: %s", TTF_GetError());
	}
	if (mTimeTextSurface == nullptr)
	{
		SDL_Log("Unable to create surface: %s", TTF_GetError());
	}
	if (mTimeTextTexture == nullptr)
	{
		SDL_Log("Unable to create texture: %s", SDL_GetError());
	}

	SDL_QueryTexture(mTimeTextTexture.get(), nullptr, nullptr, &mTimeTextRect.w, &mTimeTextRect.h);
	mTimeTextRect.x = 1000;
	mTimeTextRect.y = 10;
}

PlayGameState::~PlayGameState() 
{
}

void PlayGameState::Enter()
{
}

void PlayGameState::Exit()
{
}

void PlayGameState::ProcessKeyPressed(SDL_Keycode key)
{
	mGameWorld.mMario.ProcessKeyPressed(key, mGameWorld); 
}

void PlayGameState::ProcessKeyReleased(SDL_Keycode key)
{
	mGameWorld.mMario.ProcessKeyReleased(key);
}

void PlayGameState::Update(float elapsedTime)
{
	mGameWorld.Update(elapsedTime);
	
	if (mGameWorld.mMario.mLives == 0)
		mListener.OnLevelLose();
}

void PlayGameState::Render()
{
	mGameRenderer.BeginRenderFrame();

	/*SDL_RenderCopy(mRenderer.get(), mMarioTextTexture.get(), nullptr, &mMarioTextRect);
	SDL_RenderCopy(mRenderer.get(), mTimeTextTexture.get(), nullptr, &mTimeTextRect);*/
	
	mGameRenderer.Render(mGameWorld.mMario);
	for (const Enemy& enemy : mGameWorld.mEnemies)
		mGameRenderer.Render(enemy);

	for (const FireBall& fireBall : mGameWorld.mFireBalls)
		mGameRenderer.Render(fireBall);
	
	for (const GoldCoin& goldCoin : mGameWorld.mGoldCoins)
		mGameRenderer.Render(goldCoin);

	for (const PowerUpToLevel2& powerUpToLevel2 : mGameWorld.mPowerUpToLevel2)
		mGameRenderer.Render(powerUpToLevel2);

	for (const PowerUpToLevel3& powerUpToLevel3 : mGameWorld.mPowerUpToLevel3)
		mGameRenderer.Render(powerUpToLevel3);

	mGameRenderer.EndRenderFrame();
}

