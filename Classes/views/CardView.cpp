#include "CardView.h"

USING_NS_CC;

CardView* CardView::create(CardModel* cardModel, const std::function<void(int)>& clickCallback) {
    CardView* cardView = new (std::nothrow) CardView();
    if (cardView && cardView->init(cardModel, clickCallback)) {
        cardView->autorelease();
        return cardView;
    }
    CC_SAFE_DELETE(cardView);
    return nullptr;
}

bool CardView::init(CardModel* cardModel, const std::function<void(int)>& clickCallback) {
    if (!Node::init()) {
        return false;
    }

    _cardModel = cardModel;
    _clickCallback = clickCallback;

    _clickEnabled = true;

    if (!_cardModel) {
        return false;
    }

    // 根据卡牌模型创建精灵
    TexturePath texturePath = getCardTexturePath();
    this->_cardSprite = Sprite::create(texturePath._cardBgPath.c_str());
    
    if (_cardSprite) {
        this->addChild(_cardSprite, 0);

        
    } else {
        return false;
    }
    // 创建完整的卡牌
    auto cardFaceBig = Sprite::create(texturePath._cardFacePathBig.c_str());
    auto cardFaceSmall = Sprite::create(texturePath._cardFacePathSmall.c_str());
    auto cardSuit = Sprite::create(texturePath._cardSuitPath.c_str());
    
    _cardSprite->addChild(cardFaceBig);
    _cardSprite->addChild(cardFaceSmall);
    _cardSprite->addChild(cardSuit);

    Size parentSize = _cardSprite->getContentSize();
    //cardFaceBig->setAnchorPoint(Vec2(0.5f, 0.5f));
    cardFaceBig->setPosition(Vec2(parentSize.width / 2, parentSize.height / 2 - 30));
    cardFaceSmall->setPosition(Vec2(40, parentSize.height-50 ));
    cardSuit->setPosition(Vec2(parentSize.width -40, parentSize.height-50));

    // 设置卡牌位置
    this->setPosition(_cardModel->getPosition());

    // 设置卡牌大小
    this->setContentSize(_cardSprite->getContentSize());

    // 添加触摸事件监听
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    this->scheduleUpdate();
    return true;
}

int CardView::getCardId() const {
    if (_cardModel) {
        return _cardModel->getCardId();
    }
    return -1;
}

void CardView::setClickEnabled(bool enabled) {
    _clickEnabled = enabled;
}

TexturePath CardView::getCardTexturePath() {
    TexturePath texturePath;

    if (!_cardModel) {
        return texturePath;
    }

    CardFaceType face = _cardModel->getCardFace();
    CardSuitType suit = _cardModel->getCardSuit();

    // 构建卡牌纹理路径

    std::string suitStr;
    switch (suit) {
        case CST_CLUBS:
            suitStr = "club.png";
            break;
        case CST_DIAMONDS:
            suitStr = "diamond.png";
            break;
        case CST_HEARTS:
            suitStr = "heart.png";
            break;
        case CST_SPADES:
            suitStr = "spade.png";
            break;
        default:
            return texturePath;
    }
    texturePath._cardSuitPath = "res/suits/" + suitStr;

    std::string faceStr;
    switch (face) {
        case CFT_ACE:
            faceStr = "A.png";
            break;
        case CFT_TWO:
            faceStr = "2.png";
            break;
        case CFT_THREE:
            faceStr = "3.png";
            break;
        case CFT_FOUR:
            faceStr = "4.png";
            break;
        case CFT_FIVE:
            faceStr = "5.png";
            break;
        case CFT_SIX:
            faceStr = "6.png";
            break;
        case CFT_SEVEN:
            faceStr = "7.png";
            break;
        case CFT_EIGHT:
            faceStr = "8.png";
            break;
        case CFT_NINE:
            faceStr = "9.png";
            break;
        case CFT_TEN:
            faceStr = "10.png";
            break;
        case CFT_JACK:
            faceStr = "J.png";
            break;
        case CFT_QUEEN:
            faceStr = "Q.png";
            break;
        case CFT_KING:
            faceStr = "K.png";
            break;
        default:
            return texturePath;
    }
    if (suit == CST_CLUBS || suit == CST_SPADES){
        texturePath._cardFacePathBig = "res/number/big_black_" + faceStr;
        texturePath._cardFacePathSmall = "res/number/small_black_" + faceStr;
    }
    else {
        texturePath._cardFacePathBig = "res/number/big_red_" + faceStr;
        texturePath._cardFacePathSmall = "res/number/small_red_" + faceStr;

    }
    return texturePath;
    //return StringUtils::format("res/card_%s_%s.png", suitStr.c_str(), faceStr.c_str());
}

bool CardView::containsPointAnchoredCenter(Vec2 touchLocation, Rect boundingBox)
{
    Size size = boundingBox.size;
    bool bRet = false;

    if (touchLocation.x >= ( - size.width / 2) && 
        touchLocation.x <= ( + size.width / 2) &&
        touchLocation.y >= ( - size.height / 2) &&
        touchLocation.y <= ( + size.height / 2))
    {
        bRet = true;
    }

    return bRet;
}


bool CardView::onTouchBegan(Touch* touch, Event* event) {
    if (!_clickEnabled) {
        return false;
    }
    Vec2 touchLocation = this->convertToNodeSpace(touch->getLocation());

    Rect boundingBox = this->getBoundingBox();
    if (containsPointAnchoredCenter(touchLocation,boundingBox)) {
        if (_clickCallback) {
            _clickCallback(getCardId());
        }
        
        return true;
    }

    return false;
}


void CardView::update(float dt)  {
    if (_isOnMoving) {
        lerp(dt);
    }
}

void CardView::lerp(float lerpSpeed) {
    if (_lerpT < 1.0f) {
        _lerpT += lerpSpeed;
        _lerpT = clampf(_lerpT, 0.0f, 1.0f); // 限制 t 在 [0,1]
        Vec2 curPos = this->getPosition().lerp(_targetPosition, _lerpT);
        this->setPosition(curPos);
    }
    if (this->getPosition().distance(_targetPosition) < 0.001f) {
        this->setPosition(_targetPosition);
        _isOnMoving = false;
        _lerpT = 0;
    }
}

void CardView::startMoving(Vec2 targetPosition){
    this->_isOnMoving = true;
    this->_targetPosition = targetPosition;
}

bool CardView::getIsMoving() {
    return this->_isOnMoving;
}