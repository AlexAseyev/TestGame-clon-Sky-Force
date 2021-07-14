#include "HelloWorldScene.h"
#include "MenuScene.hpp"
#include "ui/CocosGUI.h"
#include <CCScheduler.h>

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    auto layer = HelloWorld::create();
    layer->setPhyWorld(scene->getPhysicsWorld());
    scene->addChild(layer);

    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    // setup sprite to be moved
    _spriteToMove = Sprite::create("pngegg-4.png", Rect(50,50,100,80));
    _spriteToMove->setScale(0.7);
    auto heroBody = PhysicsBody::createBox(_spriteToMove->getContentSize());
    heroBody->setDynamic(false);
    heroBody->setCategoryBitmask(1);
    heroBody->setCollisionBitmask(2);
    heroBody->setContactTestBitmask(1);
    _spriteToMove->setPhysicsBody(heroBody);
    _spriteToMove->setPosition(this->getContentSize().width/2, this->getContentSize().height/4);
    this->addChild(_spriteToMove, 1);

    _spriteToMove->setAnchorPoint(Vec2(0.5, 0));
    _spriteToMove->setFlippedX(true);

    EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
        
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
    
    auto returnMenuScene = MenuItemFont::create("Pause", CC_CALLBACK_1(HelloWorld::returnMenu, this));
    returnMenuScene->setPosition(Point(45, visibleSize.height - 20));
    auto menu_1 = Menu::create(returnMenuScene, NULL);
    menu_1->setPosition(Point(0, 0));
    this->addChild(menu_1);
    

    auto label = Label::createWithTTF("Destroy all enemies!", "fonts/Marker Felt.ttf", 30);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));

        auto delay = DelayTime::create(1.0);
        auto actionRemove = RemoveSelf::create();
        auto sequence = Sequence::create(delay, actionRemove, NULL);
        label->runAction(sequence);
        this->addChild(label, 1);
    }
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener,this);
        
 
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::heroBullet), 1.5f);
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::addMonster), 2.0f);
    
    return true;
}

void HelloWorld::createPhysicsWorld()
{
    auto body = PhysicsBody::createEdgeBox(this->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT, 3);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Point(this->getContentSize().width/2, this->getContentSize().height/2));
    edgeNode->setPhysicsBody(body);
    this->addChild(edgeNode);
}

bool HelloWorld::onContactBegin(cocos2d::PhysicsContact& contact)
{
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();
        
    // check if the bodies have collided
    if ((1 == a->getCollisionBitmask() && 3 == b->getCollisionBitmask()) || (3 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask()))
    {
        auto explosion = Sprite::create("explosion.png");
        explosion->setScale(0.3);
        explosion->setPosition(Vec2(_monster->getPositionX(), _monster->getPositionY()));
        explosion->setTag(3);
        auto actionRemove = RemoveSelf::create();
        auto sequence = Sequence::create(DelayTime::create(0.2), actionRemove, NULL);
        explosion->runAction(sequence);
        this->addChild(explosion, 1);
        
        this->removeChildByTag(2); // delete monster
        this->removeChildByTag(99); // delete bullet
    }
    else if ((1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()) || (2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask()))
    {
        auto explosion = Sprite::create("explosion.png");
        explosion->setScale(0.3);
        explosion->setPosition(Vec2(_spriteToMove->getPositionX(), _spriteToMove->getPositionY()));
        this->addChild(explosion, 1);
        
        auto labelLose = Label::createWithTTF("YOU LOSE", "fonts/Marker Felt.ttf", 30);
        labelLose->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
        labelLose->runAction(Sequence::create(DelayTime::create(1.0), RemoveSelf::create(), NULL));
        this->addChild(labelLose, 1);
        
        auto startNewGame = MenuItemFont::create("TRY AGAIN?", CC_CALLBACK_1(HelloWorld::newGame, this));
        startNewGame->setPosition(Point(this->getContentSize().width/2, this->getContentSize().height/2 - 40));
        auto menu_startNewGame = Menu::create(startNewGame, NULL);
        menu_startNewGame->setPosition(Point(0, 0));
        menu_startNewGame->setTag(6);
        this->addChild(menu_startNewGame);
        
        Director::getInstance()->stopAnimation();
    }
    
    return true;
}

void HelloWorld::heroBullet(float dt)
{
    
    //auto bullet = AutoPolygon::generatePolygon("pngegg-4.png", Rect(200,0,200,200));
    _heroBullet = Sprite::create("pngegg-4.png", Rect(250,130,100,50));
    _heroBullet->setScale(0.7);
    _heroBullet->setTag(99);
    
    auto bodyBullet = PhysicsBody::createBox(_heroBullet->getContentSize());
    bodyBullet->setDynamic(false);
    bodyBullet->setCategoryBitmask(1);
    bodyBullet->setCollisionBitmask(3);
    bodyBullet->setContactTestBitmask(1);
    _heroBullet->setPhysicsBody(bodyBullet);
    
    _heroBullet->setPosition(Vec2(_spriteToMove->getPositionX(), _spriteToMove->getPositionY() + 80));
    auto moveTo = MoveTo::create(2, Vec2(_spriteToMove->getPositionX(), this->getContentSize().height + 30));
    auto actionRemove = RemoveSelf::create();
    auto sequence = Sequence::create(moveTo, actionRemove, NULL);
    _heroBullet->runAction(RepeatForever::create(sequence));
    this->addChild(_heroBullet, 0);
    
    _heroBullet->setAnchorPoint(Vec2(0.5, 0));
    _heroBullet->setFlippedX(true);
    
}

void HelloWorld::addMonster(float dt)
{
    int width = this->getContentSize().width;
    int position = 0 + rand() % width;
    int minDuration = 2.0;
    int maxDuration = 4.0;
    int randomDuration = minDuration + rand() % maxDuration;
    auto actionMove = MoveTo::create(randomDuration, Vec2(position, -30));
    auto actionRemove = RemoveSelf::create();
    auto sequence = Sequence::create(actionMove, actionRemove, NULL);

    int temp = 1 + rand() % 4;
    if (temp == 1)
    {
        _monster = Sprite::create("pngegg-4.png", Rect(50,250,100,100));
    }
    else if (temp == 2)
    {
        _monster = Sprite::create("pngegg-4.png", Rect(20,450,160,100));
    }
    else if (temp == 3)
    {
        _monster = Sprite::create("pngegg-4.png", Rect(20,600,180,150));
    }
    else if (temp == 4)
    {
        _monster = Sprite::create("pngegg-4.png", Rect(15,850,170,100));
    }

    //_monster = Sprite::create("pngegg-4.png", Rect(15,850,170,100));
    _monster->setScale(0.5);
    _monster->setRotation(180.0f);
    _monster->setTag(2);
    auto body2 = PhysicsBody::createBox(_monster->getContentSize());
    body2->setDynamic(true);
    body2->setCategoryBitmask(3);
    body2->setCollisionBitmask(1);
    body2->setContactTestBitmask(1);
    _monster->setPhysicsBody(body2);

    // position the sprite on the center of the screen
    _monster->setPosition(Vec2(0 + rand() % width, this->getContentSize().height - 30));
    _monster->runAction(sequence);
    // add the sprite as a child to this layer
    this->addChild(_monster, 1);
    
    _monster->setAnchorPoint(Vec2(0.5, 0));
    _monster->setFlippedX(true);
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *event)
{
    Point touchLoc = touch->getLocation();
    
    if (_spriteToMove->getBoundingBox().containsPoint(touchLoc)) {
        return true;
    }
    return false;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *event)
{
    _spriteToMove->setPosition(_spriteToMove->getPosition() + touch->getDelta());
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

void HelloWorld::newGame(Ref* pSender)
{
    Director::getInstance()->startAnimation();
    Scene *gameScene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(gameScene);
}

void HelloWorld::returnMenu(Ref* pSender)
{
    auto returnMenuScene = MenuItemFont::create("Continue", CC_CALLBACK_1(HelloWorld::continueGame, this));
    returnMenuScene->setPosition(Point(this->getContentSize().width/2, this->getContentSize().height/2));
    auto menu_continueGame = Menu::create(returnMenuScene, NULL);
    menu_continueGame->setPosition(Point(0, 0));
    menu_continueGame->setTag(5);
    this->addChild(menu_continueGame);
    
    Director::getInstance()->stopAnimation();
}

void HelloWorld::continueGame(Ref* pSender)
{
    this->removeChildByTag(5); // delete continue menu
    Director::getInstance()->startAnimation();
}
