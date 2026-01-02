#include "StackView.h"

USING_NS_CC;

StackView* StackView::create(const std::function<void()>& stackClickCallback) {
    StackView* stackView = new (std::nothrow) StackView();
    if (stackView && stackView->init(stackClickCallback)) {
        stackView->autorelease();
        return stackView;
    }
    CC_SAFE_DELETE(stackView);
    return nullptr;
}

bool StackView::init(const std::function<void()>& stackClickCallback) {
    if (!Node::init()) {
        return false;
    }

    _currentTrayCardView = nullptr;
    _stackClickCallback = stackClickCallback;

    // 设置主牌区的大小和位置
    this->setContentSize(Size(1080, 580));
    this->setPosition(0, 0);

    // 添加背景
    auto background = LayerColor::create(Color4B(204, 0, 255, 255));
    background->setContentSize(this->getContentSize());
    this->addChild(background, 0);


    return true;
}
void StackView::addCardView(CardView* cardView) {
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
    this->addChild(cardView, 1);

}
void StackView::setTrayCardView(CardView* cardView) { 
    // 设置新的底牌视图
    _currentTrayCardView = cardView;
    if (_currentTrayCardView) {
        _currentTrayCardView->setPosition(Vec2(700, 300)); // 设置底牌位置
    }
}

CardView* StackView::getCardView(int cardId) const {
    auto it = _cardViewMap.find(cardId);
    if (it != _cardViewMap.end()) {
        return it->second;
    }
    return nullptr;
}


CardView* StackView::getCurrentTrayCardView() const {
    return _currentTrayCardView;
}

