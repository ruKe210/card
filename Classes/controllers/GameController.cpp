#include "GameController.h"

USING_NS_CC;

GameController::GameController() 
 {
    _gameModel = nullptr;
    _gameView = nullptr;
    _undoManager = nullptr;
    _playFieldCtrl = nullptr;
    _stackCtrl = nullptr;
    _gameModel = nullptr;
}

bool GameController::startGame(int levelId) {
    // 创建游戏数据模型
    _gameModel = new (std::nothrow) GameModel();
    if (!_gameModel) {
        return false;
    }
    _gameModel->fromJson("game.json");

    // 创建游戏视图
    _gameView = GameView::create();
    if (!_gameView) {
        return false;
    }

    // 创建回退管理器
    _undoManager = new (std::nothrow) UndoManager(
        _gameModel,
        _gameView->getPlayFieldView(),
        _gameView->getStackView()
    );
    if (!_undoManager) {
        return false;
    }
    _undoManager->init();


    // 创建主牌区控制器
    _playFieldCtrl = new (std::nothrow) PlayFieldController(
        _gameModel,
        _gameView->getPlayFieldView(),
        _gameView->getStackView(),
        _undoManager
    );
    if (!_playFieldCtrl) {
        return false;
    }
    _playFieldCtrl->init();

    // 创建备用牌堆控制器
    _stackCtrl = new (std::nothrow) StackController(
        _gameModel,
        _gameView->getPlayFieldView(),
        _gameView->getStackView(),
        _undoManager
    );
    if (!_stackCtrl) {
        return false;
    }
    _stackCtrl->init();

    // 初始化视图回调
    initViewCallbacks();

    return true;
}

GameView* GameController::getGameView() const {
    return _gameView;
}

void GameController::handleUndoClick() {
    if (_undoManager && _undoManager->canUndo()) {
        _undoManager->performUndo();
    }
}


void GameController::initViewCallbacks() {
    if (!_gameView) {
        return;
    }
     //设置回退按钮回调
    _gameView->setUndoBtnCallback([this]() {
        this->handleUndoClick();
    });
}

bool GameController::handlePlayFieldCardClick(int cardId) {
    for (auto card : _gameModel->getPlayfieldCards()) {
        auto cardView = _gameView->getPlayFieldView()->getCardView(card->getCardId());
        if (cardView->getIsMoving())
            return false;
    }
    for (auto card : _gameModel->getStackCards()) {
        auto cardView = _gameView->getStackView()->getCardView(card->getCardId());
        if (cardView->getIsMoving())
            return false;
    }
    return false;
}



