#include "json/document.h"
#include "json/filereadstream.h"

#include "AppDelegate.h"
#include "SceneFactory.hpp"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(448, 704);
static cocos2d::Size smallResolutionSize = cocos2d::Size(320, 480);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(768, 1024);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1536, 2048);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
	
	// initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("SwapIt", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("SwapIt");
#endif
        director->setOpenGLView(glview);
    }
    director->setDisplayStats(false);
    director->setAnimationInterval(1.0f / 60);

    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    
	initConfiguration(director->getWinSize().width, director->getWinSize().height);
	int column = Configuration::getInstance()->getValue("column").asInt();
	int row = Configuration::getInstance()->getValue("row").asInt();
	double coefficient = Configuration::getInstance()->getValue("coefficient").asDouble();
	float scaleFactor = MIN(director->getWinSize().width / column, director->getWinSize().height / row);
	director->setContentScaleFactor(coefficient / scaleFactor);

    register_all_packages();

    // create a scene. it's an autorelease object
	auto scene = SceneFactory::createMenuScene();//GameScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}


void AppDelegate::initConfiguration(int width, int height) {
	char buffer[128];
	FILE * configFilePointer = fopen("config.json", "r");
	rapidjson::FileReadStream configFileStream(configFilePointer, buffer, sizeof(buffer));
	rapidjson::Document configDoc;
	configDoc.ParseStream(configFileStream);
	CCASSERT(!configDoc.HasParseError(), "error occured while parsing config file");
	Configuration * config = Configuration::getInstance();
	config->setValue("column", Value(configDoc.FindMember("column")->value.GetInt()));
	config->setValue("row", Value(configDoc.FindMember("row")->value.GetInt()));
	config->setValue("coefficient", Value(configDoc.FindMember("coefficient")->value.GetDouble()));
	float fontSize = height / configDoc.FindMember("fontScaleFactor")->value.GetDouble();
	config->setValue("fontSize", Value(fontSize));
	config->setValue("font", Value(configDoc.FindMember("font")->value.GetString()));
	config->setValue("author", Value(configDoc.FindMember("author")->value.GetString()));
	config->setValue("collisionCoefficient", Value(configDoc.FindMember("collisionCoefficient")->value.GetDouble()));
	config->setValue("fadeInTime", Value(configDoc.FindMember("fadeInTime")->value.GetDouble()));
	auto mainMenuLines = ValueVector();
	auto mainMenuLinesMember = configDoc.FindMember("mainMenuLines");
	for (auto iter = mainMenuLinesMember->value.Begin(); iter != mainMenuLinesMember->value.End(); ++iter) {
		auto leftValue = Value(iter->FindMember("l")->value.GetString());
		auto middleValue = Value(iter->FindMember("m")->value.GetString());
		auto rightValue = Value(iter->FindMember("r")->value.GetString());
		mainMenuLines.push_back(Value(ValueVector({ leftValue, middleValue, rightValue })));
	}
	config->setValue("mainMenuLines", Value(mainMenuLines));
	fclose(configFilePointer);
}