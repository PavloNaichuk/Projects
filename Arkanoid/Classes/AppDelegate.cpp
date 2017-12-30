#include "AppDelegate.h"
#include "MainScene.h"

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

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 600);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

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
    auto glview = director->getOpenGLView();
    if (glview == nullptr)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Arkanoid", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("Arkanoid");
#endif
        director->setOpenGLView(glview);
    }

    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    
	auto frameSize = glview->getFrameSize();    
    if (frameSize.height > mediumResolutionSize.height)
    {
		float scaleFactor = MIN(largeResolutionSize.height / designResolutionSize.height,
			largeResolutionSize.width / designResolutionSize.width);
		
		director->setContentScaleFactor(scaleFactor);
    }
    else if (frameSize.height > smallResolutionSize.height)
    {
		float scaleFactor = MIN(mediumResolutionSize.height / designResolutionSize.height,
			mediumResolutionSize.width / designResolutionSize.width);
		
		director->setContentScaleFactor(scaleFactor);
    }
    else
    {
		float scaleFactor = MIN(smallResolutionSize.height / designResolutionSize.height,
			smallResolutionSize.width / designResolutionSize.width);
		
		director->setContentScaleFactor(scaleFactor);
    }
	
    auto mainScene = MainScene::create();
    director->runWithScene(mainScene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
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

// This function will be called when the app is active again
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
