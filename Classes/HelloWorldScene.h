
#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <map>

class HelloWorld : public cocos2d::Scene
{
private:
    cocos2d::Sprite *_spriteToMove;
    cocos2d::Sprite *_monster;
    cocos2d::Sprite *_heroBullet;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    cocos2d::PhysicsWorld* m_world;
    void setPhyWorld(cocos2d::PhysicsWorld* world){m_world = world;}
    //bool onContactBegin(cocos2d::PhysicsContact& contact);
    
    void createPhysicsWorld();
    bool onContactBegin(cocos2d::PhysicsContact& contact);

    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void heroBullet(float dt);
    void addMonster(float dt);
    void newGame(Ref* pSender);
    void returnMenu(Ref* pSender);
    void continueGame(Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif __HELLOWORLD_SCENE_H__
