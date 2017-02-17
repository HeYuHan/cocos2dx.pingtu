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
	_backgroundTex(nullptr),
    _swapCard(nullptr),
	_moveStep(nullptr),
	_moveStepCount(0)
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
	_moveStep = Label::create();
	_moveStep->setString("移动:0");
	_moveStep->setColor(Color3B::RED);
	_moveStep->setSystemFontSize(16);
	Vec2 center = getCenterPosition();
	center.y *= 2;
	center.y -= 10;
	_moveStep->setPosition(center);
	mainLayer->addChild(_moveStep, 100);
	setGameSplit(4, 4);
	initGame();
    randMoveCard(50);
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
			if (frame)
			{
				_backgoundTexRect = frame->getRect();
				_backgroundTex = frame->getTexture();
			}
			break;
		}
		
	case GameApp::R_TEXTURE:
		{
            _backgroundTex=Director::getInstance()->getTextureCache()->addImage(_backgroundRes);
			if (_backgroundTex)
			{
				_backgoundTexRect.origin = Vec2::ZERO;
				_backgoundTexRect.size = _backgroundTex->getContentSize();
			}

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
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    _cardStartPosition = (visibleSize - _backgroundContentSize) / 2;
    _cardStartPosition+=origin;
	
}

void GameApp::setGameSplit(int splitx, int splity)
{
	_splitx = splitx;
	_splity = splity;
}
void GameApp::initGame()
{
	setBackGroundResource("background.png", R_TEXTURE);
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
			cardSptire->setContentSize(Size(cx-1, cy-1));
			cardSptire->setPosition(Vec2(x*cx + cx / 2, y*cy + cy / 2) + _cardStartPosition);
			if (x + y == 0)
			{
                cardSptire->setColor(Color3B::GREEN);
				card->setSwap(true);
                _swapCard=card;
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
			addMoveStep();
			return true;
		}
		return false;
	};
	card->getSprite()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, card->getSprite());
}
void GameApp::onCardTouch(Card* card)
{
	if(swapCardPosition(card))
    {
        if(isVictory())onVictory();
    }
    
	
}
void GameApp::addMoveStep()
{
	char buffer[128];
	sprintf(buffer, "步数: %d", _moveStepCount++);
	_moveStep->setString(buffer);
}
bool GameApp::swapCardPosition(Card *card)
{
    if(!card||!_swapCard)return false;
    int px1 = card->getPosX();
    int py1 = card->getPosY();
    int px2 = _swapCard->getPosX();
    int py2 = _swapCard->getPosY();
    
    if (((abs(px1-px2)==1&&py1==py2)||((abs(py1-py2)==1)&&px1==px2)))
    {
        card->setPosIndex(px2, py2);
        _swapCard->setPosIndex(px1, py1);
        Vec2 pos = card->getSprite()->getPosition();
        card->getSprite()->setPosition(_swapCard->getSprite()->getPosition());
        _swapCard->getSprite()->setPosition(pos);
        return true;
    }
    return false;
}
int GameApp::random(int min, int max,bool resetSeed)
{
    if(resetSeed)
    {
        //获取系统时间
        struct timeval now;
        gettimeofday(&now, NULL);
        
        //初始化随机种子
        //timeval是个结构体，里边有俩个变量，一个是以秒为单位的，一个是以微妙为单位的
        unsigned rand_seed = (unsigned)(now.tv_sec*1000 + now.tv_usec/1000);    //都转化为毫秒
        srand(rand_seed);
    }
    
    int range=max-min;
    int _rand = rand()%range+min;
    return _rand;
}
void GameApp::randMoveCard(int moveCount)
{
    Card* moveCard=nullptr;
    while (moveCount-->0) {
        Card* findCard=nullptr;
        int direction=random(0, 4,moveCard==nullptr);
        switch (direction) {
            case 0:
                findCard=getCardByIndex(_swapCard->getPosX()+1, _swapCard->getPosY());
                break;
            case 1:
                findCard=getCardByIndex(_swapCard->getPosX()-1, _swapCard->getPosY());
                break;
            case 2:
                findCard=getCardByIndex(_swapCard->getPosX(), _swapCard->getPosY()+1);
                break;
            case 3:
                findCard=getCardByIndex(_swapCard->getPosX(), _swapCard->getPosY()-1);
                break;
        }
        if(findCard==moveCard||!swapCardPosition(findCard))
        {
            moveCount--;
            continue;
        }
        moveCard=findCard;
    }
}
Card* GameApp::getCardByIndex(int indexx, int indexy)
{
    for(vector<Card*>::iterator i=_cardList.begin();i!=_cardList.end();i++)
    {
        Card* find=*i;
        if(find->getPosX()==indexx&&find->getPosY()==indexy)
        {
            return find;
        }
    }
    return nullptr;
}
bool GameApp::isVictory()
{
    
    for(vector<Card*>::iterator i=_cardList.begin();i!=_cardList.end();i++)
    {
        if(!(*i)->isTruePos())
        {
            return false;
        }
    }
    return true;
}
Vec2 GameApp::getCenterPosition()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 center = (origin + visibleSize) / 2;
	return center;
}
void GameApp::onVictory()
{
	float cx = _backgroundContentSize.width / _splitx;
	float cy = _backgroundContentSize.height / _splity;
	for (auto *card : _cardList)
	{
		card->getSprite()->setContentSize(Size(cx, cy));
		card->getSprite()->setColor(Color3B::WHITE);
	}
    log("onVictory:%d",1);
}
void GameApp::clearCards()
{
	vector<Card*>::iterator i = _cardList.begin();
	while (i!=_cardList.end())
	{
		i = _cardList.erase(i);
	}
}
