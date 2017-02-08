//
//  GameLayer.cpp
//  pingtu
//
//  Created by heyuhan on 17/2/8.
//
//

#include "GameApp.h"
GameApp gGame;
GameApp* GameApp::getInstance()
{
    return &gGame;
}
GameApp::GameApp():
    mainLayer(nullptr)
{
    
}
GameApp::~GameApp()
{
    
}
bool GameApp::initWithScene(cocos2d::Scene *scene)
{
    if(!scene)return false;
    mainScene=scene;
    mainLayer=LayerColor::create(Color4B::GRAY);
    mainScene->addChild(mainLayer);
    return true;
}
