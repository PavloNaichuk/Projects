#include "GameTimer.h"

GameTimer::GameTimer()
    : mStarted(false)
    , mPaused(false)
    , mElapsedMs(0)
{
}

void GameTimer::start()
{
    if (mStarted)
    {
        mPaused = false;
        mElapsedMs = 0;
        mTime.restart();
    }
    else
    {
        mTime.start();
        mStarted = true;
    }
}

void GameTimer::pause()
{
    mPaused = true;
}

void GameTimer::resume()
{
    mPaused = false;
    mTime.restart();
}

void GameTimer::update()
{
    if (!mPaused)
    {
        mElapsedMs += mTime.elapsed();
        mTime.restart();
        emit timeChanged(mElapsedMs);
    }
}
