//
//  MenuScene.cpp
//  TestProject
//
//  Created by Umirai on 06.07.2021.
//

#include "MenuScene.hpp"
#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    Director::getInstance()->runWithScene(scene);
    // return the scene
    return scene;
}

bool MenuScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto menu_item_1 = MenuItemFont::create("Play", CC_CALLBACK_1(MenuScene::Play, this));
    auto menu_item_2 = MenuItemFont::create("About the program", CC_CALLBACK_1(MenuScene::Settings, this));
    auto menu_item_3 = MenuItemFont::create("Exit", CC_CALLBACK_1(MenuScene::Exit, this));

    menu_item_1->setPosition(Point(visibleSize.width / 2, (visibleSize.height / 4) * 3));
    menu_item_2->setPosition(Point(visibleSize.width / 2, (visibleSize.height / 4) * 2));
    menu_item_3->setPosition(Point(visibleSize.width / 2, (visibleSize.height / 4) * 1));
        
    auto *menu_1 = Menu::create(menu_item_1, menu_item_2, menu_item_3, NULL);
    menu_1->setPosition(Point(0, 0));
    this->addChild(menu_1);
    
    return true;
}

void MenuScene::Play(cocos2d::Ref *pSender)
{
    CCLOG("Play");
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(scene);
    //Director::getInstance( )->replaceScene( TransitionFade::create( TRANSITION_TIME, scene ) );
}

void MenuScene::Settings(cocos2d::Ref *pSender)
{
    CCLOG("About the program");
    
}

void MenuScene::Exit(cocos2d::Ref *pSender)
{
    CCLOG("Exit");
    Director::getInstance()->end();
}
