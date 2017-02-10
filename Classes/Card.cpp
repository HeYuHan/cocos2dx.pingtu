//
//  Card.cpp
//  pingtu
//
//  Created by heyuhan on 17/2/8.
//
//

#include "Card.h"
USING_NS_CC;
Card::Card():
    _cardImg(nullptr),
    _textureX(0),
    _textureY(0),
    _posX(0),
    _posY(0),
	_isSwap(false)
{
    
}
Card::~Card()
{
    if(_cardImg&&_cardImg->getParent())
    {
        _cardImg->getParent()->removeChild(_cardImg);
    }
}
bool Card::init(cocos2d::Layer *layer)
{
    if(!layer)return false;
    _cardImg=Sprite::create();
    layer->addChild(_cardImg);
    return true;
}
bool Card::isTruePos()
{
    return _textureX==_posX&&_textureY==_posY;
}
Sprite* Card::getSprite()
{
	return _cardImg;
}
void Card::setCardSize(cocos2d::Size &size)
{
    if(!_cardImg)return;
    _cardImg->setContentSize(_cardSize);
}
void Card::setCardTextureRect(cocos2d::Rect &rect)
{
    if(!_cardImg)return;
    _cardImg->setTextureRect(_textureRect);
}
void Card::setTextureIndex(int x, int y)
{
    _textureX=x;
    _textureY=y;
}
void Card::setPosIndex(int x, int y)
{
    _posX=x;
    _posY=y;
}
void Card::setPosition(float x, float y)
{
    if(!_cardImg)return;
    _cardImg->setPosition(x,y);
}
void Card::setSwap(bool c)
{
	_isSwap = c;
}
bool Card::isSwap()
{
	return _isSwap;
}
int Card::getPosX()
{
	return _posX;
}
int Card::getPosY()
{
	return _posY;
}


























