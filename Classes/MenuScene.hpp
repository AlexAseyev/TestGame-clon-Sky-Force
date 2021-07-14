
//
//  MenuScene.hpp
//  TestProject
//
//  Created by Umirai on 06.07.2021.
//

#ifndef MenuScene_hpp
#define MenuScene_hpp

#include <stdio.h>
#include "cocos2d.h"

class MenuScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void Play(Ref *pSender);
    void Settings(Ref *pSender);
    void Exit(Ref *pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MenuScene);
    
};


#endif /* MenuScene_hpp */
