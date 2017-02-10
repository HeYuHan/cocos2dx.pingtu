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
#include "Card.h"
#include <vector>
USING_NS_CC;
class GameApp
{
	enum ResourceType
	{
		R_ATLAS,
		R_TEXTURE,
		R_URL
	};
public:
    GameApp();
    ~GameApp();
    static GameApp* getInstance();
    virtual bool initWithScene(Scene* scene);
	void setBackGroundResource(const std::string &res, ResourceType type);
	void setGameSplit(int splitx, int splity);
	void clearCards();
	void initGame();
	void startGame();
	void onCardTouch(Card* card);
	void setCardTouchEvent(Card* card);
private:
	void initCard();
private:
    Layer* mainLayer;
    Scene* mainScene;
    std::string _backgroundRes;
	std::vector<Card*> _cardList;
	int _splitx, _splity;
	ResourceType _rType;
	Rect _backgoundTexRect;
	Texture2D* _backgroundTex;
	Vec2 _cardStartPosition;
	Size _backgroundContentSize;
};


#endif /* GameLayer_hpp */
