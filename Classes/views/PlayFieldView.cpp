#include "PlayFieldView.h"

USING_NS_CC;

PlayFieldView* PlayFieldView::create(const std::function<void(int)>& cardClickCallback) {
    PlayFieldView* playFieldView = new (std::nothrow) PlayFieldView();
    if (playFieldView && playFieldView->init(cardClickCallback)) {
        playFieldView->autorelease();
        return playFieldView;
    }
    CC_SAFE_DELETE(playFieldView);
    return nullptr;
}

bool PlayFieldView::init(const std::function<void(int)>& cardClickCallback) {
    if (!Node::init()) {
        return false;
    }

    _cardClickCallback = cardClickCallback;
    _cardViewMap.clear();

    // 设置主牌区的大小和位置
    this->setContentSize(Size(1080 , 1500));
    this->setPosition(0, 580);

    // 添加背景
    auto background = LayerColor::create(Color4B(204, 117, 29, 255));
    background->setContentSize(this->getContentSize());
    this->addChild(background, 1);

    return true;
}

void PlayFieldView::addCardView(CardView* cardView) {
    if (!cardView) {
        return;
    }

    int cardId = cardView->getCardId();

    // 检查该卡牌ID是否已存在
    if (_cardViewMap.find(cardId) != _cardViewMap.end()) {
        return;
    }

    // 添加卡牌到映射表
    _cardViewMap[cardId] = cardView;

    // 添加卡牌视图到场景
    Scene* curScene = Director::getInstance()->getRunningScene();
    curScene->addChild(cardView, 1);
    cardView->setPosition(cardView->getPosition() + this->getPosition());
}

void PlayFieldView::removeCardView(int cardId) {
    auto it = _cardViewMap.find(cardId);
    if (it != _cardViewMap.end()) {
        CardView* cardView = it->second;
        cardView->removeFromParent();
        _cardViewMap.erase(it);
    }
}

CardView* PlayFieldView::getCardView(int cardId) const {
    auto it = _cardViewMap.find(cardId);
    if (it != _cardViewMap.end()) {
        return it->second;
    }
    return nullptr;
}
