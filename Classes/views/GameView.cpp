#pragma execution_character_set("utf-8")
#include "GameView.h"

USING_NS_CC;

GameView* GameView::create() {
    GameView* gameView = new (std::nothrow) GameView();
    if (gameView && gameView->init()) {
        gameView->autorelease();
        return gameView;
    }
    CC_SAFE_DELETE(gameView);
    return nullptr;
}

bool GameView::init() {
    if (!Node::init()) {
        return false;
    }

    // 初始化主牌区视图
    _playFieldView = PlayFieldView::create([](int cardId) {
    });
    if (_playFieldView) {
        //_playFieldView->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        this->addChild(_playFieldView, 1);
    }

    // 初始化备用牌堆+底牌视图
    _stackView = StackView::create([]() {
    });
    if (_stackView) {
        //_stackView->setPosition(origin.x + 100, origin.y + 100);
        this->addChild(_stackView, 1);
    }

    // 初始化回退按钮
    _undoBtn = Label::createWithTTF("回退", "fonts/msyh.ttf", 70);
    _undoBtn->setColor(Color3B::WHITE);          // 正常颜色（白色）
    _undoBtn->setPosition(950,300); 

    this->addChild(_undoBtn, 3);              // 添加到场景（层级置顶）
    auto listener = EventListenerTouchOneByOne::create();
    // 关闭触摸吞噬（避免影响其他节点）
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(GameView::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,_undoBtn);

    return true;
}

PlayFieldView* GameView::getPlayFieldView() const {
    return _playFieldView;
}

StackView* GameView::getStackView() const {
    return _stackView;
}

bool GameView::onTouchBegan(Touch* touch, Event* event) {

    Vec2 touchLocalPos = _undoBtn->convertToNodeSpace(touch->getLocation());
    Rect labelRect = _undoBtn->getBoundingBox();
    labelRect.origin = Vec2(0, 0);
    //判断是否点击到Label范围内
    if (labelRect.containsPoint(touchLocalPos)) {
        if(_clickCallback)
            _clickCallback();
        return true; 
    }

    return false;
}

void GameView::setUndoBtnCallback(std::function<void()> clickCallback) {
    _clickCallback = clickCallback;
}