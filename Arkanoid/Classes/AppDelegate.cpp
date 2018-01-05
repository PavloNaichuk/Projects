#include "AppDelegate.h"
#include "MainScene.h"
#include "OptionsScene.h"
#include "PlayScene.h"
#include "GameWinScene.h"
#include "GameLoseScene.h"
#include "ConfigManager.h"
#include "GameFactory.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs;
	glContextAttrs.redBits = 8;
	glContextAttrs.greenBits = 8;
	glContextAttrs.blueBits = 8;
	glContextAttrs.alphaBits = 8;
	glContextAttrs.depthBits = 24;
	glContextAttrs.stencilBits = 8;
	
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    auto director = Director::getInstance();
	
	auto configManager = ConfigManager::getInstance();
	const Size& worldSize = configManager->getWorldSize();

    auto glview = director->getOpenGLView();
    if (glview == nullptr)
	{
        glview = GLViewImpl::createWithRect("Arkanoid", Rect(0, 0, worldSize.width, worldSize.height));
        director->setOpenGLView(glview);
    }
    glview->setDesignResolutionSize(worldSize.width, worldSize.height, ResolutionPolicy::NO_BORDER);

	auto spriteFrameCache = SpriteFrameCache::getInstance();
	spriteFrameCache->addSpriteFramesWithFile("images/ImageAtlas.plist");

	startGame();
    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}

void AppDelegate::startGame()
{
	auto director = Director::getInstance();
	director->runWithScene(MainScene::create(this));
}

void AppDelegate::playGame()
{
	auto director = Director::getInstance();
	director->replaceScene(GameFactory::createTransitionScene(PlayScene::create(this)));
}

void AppDelegate::replayGame()
{
	auto director = Director::getInstance();
	director->replaceScene(GameFactory::createTransitionScene(PlayScene::create(this)));
}

void AppDelegate::exitGame()
{
	auto director = Director::getInstance();
	director->end();
}

void AppDelegate::loadMainMenu()
{
	auto director = Director::getInstance();
	director->replaceScene(GameFactory::createTransitionScene(MainScene::create(this)));
}

void AppDelegate::loadOptionsMenu()
{
	auto director = Director::getInstance();
	director->replaceScene(GameFactory::createTransitionScene(OptionsScene::create(this)));
}

void AppDelegate::notifyGameWin()
{
	auto director = Director::getInstance();
	director->replaceScene(GameFactory::createTransitionScene(GameWinScene::create(this)));
}

void AppDelegate::notifyGameLose()
{
	auto director = Director::getInstance();
	director->replaceScene(GameFactory::createTransitionScene(GameLoseScene::create(this)));
}
