#include "StackController.h"

USING_NS_CC;

StackController::StackController(GameModel* gameModel, PlayFieldView* playFieldView, StackView* stackView, UndoManager* undoManager){
    _gameModel = gameModel;
    _playFieldView = playFieldView;
    _stackView = stackView;
    _undoManager = undoManager;
}

void StackController::init() {
    if (!_gameModel || !_stackView || !_undoManager) {
        return;
    }

    // 设置初始底牌
    const std::vector<CardModel*>& stackCards = _gameModel->getStackCards();
    if (!stackCards.empty()) {
        CardModel* initialTrayCard = stackCards[stackCards.size()-1];
        _gameModel->setCurrentTrayCard(initialTrayCard);

        // 创建底牌视图
        CardView* trayCardView = CardView::create(initialTrayCard, [this](int cardId) {
            //this->handleStackClick();
           
        });
        
        if (trayCardView) {
            _stackView->setTrayCardView(trayCardView);
            // trayCardView->setClickEnabled();
            _stackView->addCardView(trayCardView);
            trayCardView->setClickEnabled(false);

            //initialTrayCard->setPosition(trayCardView->getPosition());
        }

        for (int i = 0; i < (stackCards.size() - 1); i++)
        {
            CardView* cardView = CardView::create(stackCards[i], [this](int cardId) {
                this->handleStackClick();
            });
            cardView->setPosition(Vec2(250 + i * 150, 300));
            _stackView->addCardView(cardView);
            
            if(i== stackCards.size() - 2)
                cardView->setClickEnabled(true);
            else
                cardView->setClickEnabled(false);

        }
        
    }

    // 更新备用牌堆显示
    int stackCount = _gameModel->getStackCards().size() - 1; // 减1是因为第一张已作为底牌


    _undoManager->handleDrawUndo([this](const UndoRecord& record) {
        this->undoStackOperation(record);
        });

}

bool StackController::handleStackClick() {

    for (auto card : _gameModel->getPlayfieldCards()) {
        auto cardView = _playFieldView->getCardView(card->getCardId());
        if (cardView->getIsMoving())
            return false;
    }
    for (auto card : _gameModel->getStackCards()) {
        auto cardView = _stackView->getCardView(card->getCardId());
        if (cardView->getIsMoving())
            return false;
    }


    // 从备用牌堆抽牌
    CardModel* newTrayCard = _gameModel->drawStackCard();


    // 获取当前底牌（用于回退）
    CardModel* trayCard = _gameModel->getCurrentTrayCard();


    auto newTrayCardView = _playFieldView->getCardView(newTrayCard->getCardId());
    if (!newTrayCardView) {
        newTrayCardView = _stackView->getCardView(newTrayCard->getCardId());
    }

    auto trayCardView = _playFieldView->getCardView(trayCard->getCardId());
    if (!trayCardView) {
        trayCardView = _stackView->getCardView(trayCard->getCardId());
    }
    

    Vec2 newTrayCardViewTargetPosition = trayCardView->getPosition();
    Vec2 trayCardTargetPosition = trayCardView->getPosition() + Vec2(0, -500);

    newTrayCardView->setClickEnabled(false);
    trayCardView->setClickEnabled(false);

    recordDrawUndo(newTrayCard, trayCard, newTrayCardView->getPosition(), trayCardView->getPosition());

    newTrayCardView->startMoving(newTrayCardViewTargetPosition);
    trayCardView->startMoving(trayCardTargetPosition);

    _gameModel->setCurrentTrayCard(newTrayCard);
    const auto nextCard = _gameModel->peekStackCard();
    if (nextCard){
        _stackView->getCardView(nextCard->getCardId())->setClickEnabled(true);
    }
     
    return true;
}

void StackController::recordDrawUndo(CardModel* playCard, CardModel* originalTrayCard, Vec2 operatedCardPosition, Vec2 originalTrayCardPosition) {
    if (!_undoManager || !playCard || !originalTrayCard) {
        return;
    }

    // 创建回退记录
    UndoRecord undoRecord;
    undoRecord.opType = UNDO_OP_STACK_DRAW;
    undoRecord.originalTrayCard = originalTrayCard;
    undoRecord.operatedCard = playCard;
    undoRecord.operatedCardPosition = operatedCardPosition;
    undoRecord.originalTrayCardPosition = originalTrayCardPosition;

    // 添加到回退管理器
    _undoManager->addUndoRecord(undoRecord);
}

StackView* StackController::getStackView() {
    return this->_stackView;
}

void StackController::undoStackOperation(const UndoRecord& record) {
    if (!_playFieldView || !_gameModel) {
        return;
    }

    if (record.opType != UNDO_OP_STACK_DRAW) {
        return;
    }

    if (!record.operatedCard) {
        return;
    }

    CardModel* playCard = record.operatedCard;
    CardModel* trayCard = record.originalTrayCard;
    auto playCardView = _playFieldView->getCardView(playCard->getCardId());
    if (!playCardView) {
        playCardView = _stackView->getCardView(playCard->getCardId());
    }

    auto trayCardView = _playFieldView->getCardView(trayCard->getCardId());
    if (!trayCardView) {
        trayCardView = _stackView->getCardView(trayCard->getCardId());
    }

    Vec2 playCardTargetPosition = record.operatedCardPosition;
    Vec2 trayCardTargetPosition = record.originalTrayCardPosition;
    playCardView->setClickEnabled(true);
    trayCardView->setClickEnabled(false);
    _gameModel->setCurrentTrayCard(trayCard);


    const auto preCard = _gameModel->peekStackCard();
    if (preCard) {
        _stackView->getCardView(preCard->getCardId())->setClickEnabled(false);
    }

    _gameModel->pushStackCard();

    playCardView->startMoving(playCardTargetPosition);
    trayCardView->startMoving(trayCardTargetPosition);
 
}
