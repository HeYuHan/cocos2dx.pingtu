//
//  Card.hpp
//  pingtu
//
//  Created by heyuhan on 17/2/8.
//
//

#ifndef CARD_H
#define CARD_H
#include "cocos2d.h"
class Card
{
public:
    Card();
    ~Card();
    bool init(cocos2d::Layer *layer);
    void setPosIndex(int x,int y);
	int getPosX();
	int getPosY();
    void setTextureIndex(int x,int y);
    void setCardSize(cocos2d::Size &size);
    void setCardTextureRect(cocos2d::Rect &rect);
    void setPosition(float x,float y);
	void setSwap(bool c);
	bool isSwap();
    bool isTruePos();
	cocos2d::Sprite* getSprite();
private:
    cocos2d::Sprite* _cardImg;
    int _textureX,_textureY;
    int _posX,_posY;
    cocos2d::Rect _textureRect;
    cocos2d::Size _cardSize;
	bool _isSwap;
};

#endif /* Card_hpp */
