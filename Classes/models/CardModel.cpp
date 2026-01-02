#include "CardModel.h"

using namespace cocos2d;

CardModel::CardModel(int cardId, CardFaceType face, CardSuitType suit, const Vec2& position)
{
    this->_cardId = cardId;
    this->_cardFace = face;
    this->_cardSuit = suit;
    this->_position = position;
}

int CardModel::getCardId() const
{
    return _cardId;
}

CardFaceType CardModel::getCardFace() const
{
    return _cardFace;
}

CardSuitType CardModel::getCardSuit() const
{
    return _cardSuit;
}

Vec2 CardModel::getPosition() const
{
    return _position;
}

void CardModel::setPosition(const Vec2& position)
{
    _position = position;
}


PileUp CardModel::getPileUp()  {
    return this->_pileUp;
}
// 设置位置
void CardModel::setPileUp(const PileUp& pileUp) {
    this->_pileUp = pileUp;
}