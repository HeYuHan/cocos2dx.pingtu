//
//  GameLayer.cpp
//  pingtu
//
//  Created by heyuhan on 17/2/8.
//
//

#include "GameApp.h"
using namespace std;
GameApp gGame;
GameApp* GameApp::getInstance()
{
    return &gGame;
}
GameApp::GameApp():
    mainLayer(nullptr),
	_splitx(0),
	_splity(0),
	_rType(R_TEXTURE),
	_backgroundTex(nullptr)
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
	setGameSplit(2, 2);
	initGame();
    return true;
}
void GameApp::setBackGroundResource(const std::string &res, ResourceType type)
{
	_backgroundRes = res;
	_rType = type;
	switch (_rType)
	{
	case GameApp::R_ATLAS:
		{
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(res);
			_backgoundTexRect = frame->getRect();
			_backgroundTex = frame->getTexture();
			break;
		}
		
	case GameApp::R_TEXTURE:
		{
			_backgroundTex = TextureCache::getInstance()->addImage(_backgroundRes);
			_backgoundTexRect.origin = Vec2::ZERO;
			_backgoundTexRect.size = _backgroundTex->getContentSize();
			break;
		}
		
	case GameApp::R_URL:
		break;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float px = visibleSize.width / _backgoundTexRect.size.width;
	float py = visibleSize.height / _backgoundTexRect.size.height;
	float scale = px > py ? py : px;
	_backgroundContentSize = _backgoundTexRect.size*scale;
	_cardStartPosition = (visibleSize - _backgroundContentSize) / 2;
	
}

void GameApp::setGameSplit(int splitx, int splity)
{
	_splitx = splitx;
	_splity = splity;
}
void GameApp::initGame()
{
	
	setBackGroundResource("HelloWorld.png", R_TEXTURE);
	initCard();
}
void GameApp::initCard()
{
	clearCards();
	if (_splitx == 0 || _splity == 0)return;
	float dx = _backgoundTexRect.size.width / _splitx;
	float dy = _backgoundTexRect.size.height / _splity;
	float cx = _backgroundContentSize.width / _splitx;
	float cy = _backgroundContentSize.height / _splity;
	for (int x = 0; x < _splitx; x++)
	{
		for (int y = 0; y < _splity; y++)
		{
			Card* card = new Card();
			_cardList.push_back(card);
			card->init(mainLayer);
			card->setTextureIndex(x, y);
			card->setPosIndex(x, y);
			Sprite* cardSptire = card->getSprite();
			Rect spriteRect;
			spriteRect.origin.x = _backgoundTexRect.origin.x + x*dx;
			spriteRect.origin.y = _backgoundTexRect.origin.y - (y + 1)*dy + _backgoundTexRect.size.height;
			spriteRect.size.setSize(dx, dy);
			cardSptire->setTexture(_backgroundTex);
			cardSptire->setTextureRect(spriteRect);
			cardSptire->setContentSize(Size(cx, cy));
			cardSptire->setPosition(Vec2(x*cx + cx / 2, y*cy + cy / 2) + _cardStartPosition);
			if (x + y == 0)
			{
				cardSptire->setColor(Color3B(100, 100, 100));
				card->setSwap(true);
				continue;
			}
			setCardTouchEvent(card);
			
			
		}
	}
}
void GameApp::setCardTouchEvent(Card* card)
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&, card](Touch* t, Event* e){
		Rect posRect;
		posRect.size = card->getSprite()->getContentSize();
		posRect.origin = card->getSprite()->getPosition() - posRect.size / 2;
		Vec2 touchPos = t->getLocation();
		if (posRect.containsPoint(touchPos))
		{
			onCardTouch(card);
			return true;
		}
		return false;
	};
	card->getSprite()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, card->getSprite());
}
void GameApp::onCardTouch(Card* card)
{
	for (vector<Card*>::iterator i = _cardList.begin(); i != _cardList.end(); i++)
	{
		Card* empty = *i;
		int px1 = card->getPosX();
		int py1 = card->getPosY();
		int px2 = empty->getPosX();
		int py2 = empty->getPosY();

		if (empty->isSwap()&&(abs(px1-px2)==1||abs(py1-py2)==1))
		{
			card->setPosIndex(px2, py2);
			empty->setPosIndex(px1, py1);
			Vec2 pos = card->getSprite()->getPosition();
			card->getSprite()->setPosition(empty->getSprite()->getPosition());
			empty->getSprite()->setPosition(pos);
		}
	}
	CCLOGINFO("card touch:%d",1);
}
void GameApp::clearCards()
{
	vector<Card*>::iterator i = _cardList.begin();
	while (i!=_cardList.end())
	{
		i = _cardList.erase(i);
	}
}
