//
//  GameLayer.hpp
//  pingtu
//
//  Created by heyuhan on 17/2/8.
//
//

#ifndef GAMEAPP_H
#define GAMEAPP_H
#include "cocos2d.h"
USING_NS_CC;
class GameApp
{
public:
    GameApp();
    ~GameApp();
    static GameApp* getInstance();
    virtual bool initWithScene(Scene* scene);
    void setBackGroundSpriteName(std::string &name);
    void setBackGroundSpriteUrl(std::string &url);
private:
    Layer* mainLayer;
    Scene* mainScene;
    std::string _backgroundSpriteName;
    std::string _backgroundSpriteUrl;
};


#endif /* GameLayer_hpp */
