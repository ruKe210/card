#include "PlayFieldController.h"

USING_NS_CC;

PlayFieldController::PlayFieldController(GameModel* gameModel, PlayFieldView* playFieldView, StackView* stackView, UndoManager* undoManager){
    _gameModel = gameModel;
    _playFieldView = playFieldView;
    _stackView = stackView;
    _undoManager = undoManager;

}

void PlayFieldController::init() {
    if (!_gameModel || !_playFieldView || !_undoManager) {
        return;
    }


    // 添加主牌区所有卡牌视图，并设置点击回调
    const std::vector<CardModel*>& cards = _gameModel->getPlayfieldCards();
    for (CardModel* cardModel : cards) {
        if (cardModel) {
            // 创建卡牌视图
            CardView* cardView = CardView::create(cardModel, [this](int cardId) {
                this->handleCardClick(cardId);
            });

            if (cardView) {
                _playFieldView->addCardView(cardView);
                findPileUp(cardView);
            }
        }
    }

    _undoManager->handleMatchUndo([this](const UndoRecord& record) {
        this->undoCardOperation(record);
     });
}

void PlayFieldController::findPileUp(CardView* cardViewCurrnet)
{
    Rect cardViewCurrnetRect = cardViewCurrnet->getBoundingBox();
    const auto playfieldCards = _gameModel->getPlayfieldCards();
    PileUp cardViewCurrnetRectPileUp = playfieldCards.at(cardViewCurrnet->getCardId())->getPileUp();

    for (const auto* playfieldCard : playfieldCards){
        if (cardViewCurrnet->getCardId() <= playfieldCard->getCardId())
            break;

        auto* cardView = _playFieldView->getCardView(playfieldCard->getCardId());
        Rect cardViewRect = cardView->getBoundingBox();
        if (cardViewCurrnetRect.intersectsRect(cardViewRect)){
            cardView->setClickEnabled(false);
            PileUp cardViewRectPileUp = playfieldCards.at(cardView->getCardId())->getPileUp();
            cardViewRectPileUp._parentCount++;
            cardViewCurrnetRectPileUp._childList.push_back(cardView->getCardId());
            playfieldCards.at(cardView->getCardId())->setPileUp(cardViewRectPileUp);
            playfieldCards.at(cardViewCurrnet->getCardId())->setPileUp(cardViewCurrnetRectPileUp);

        }
    }

}

PlayFieldView* PlayFieldController::getPlayFieldView()
{
    return this->_playFieldView;
}


bool PlayFieldController::handleCardClick(int cardId) {

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


    CardModel* playCard = _gameModel->findCardById(cardId);
    CardModel* trayCard = _gameModel->getCurrentTrayCard();

    if (!performCardMatch(playCard, trayCard)) {
        return false;
    }



    auto playCardView = _playFieldView->getCardView(playCard->getCardId());
    if (!playCardView) {
        playCardView = _stackView->getCardView(playCard->getCardId());
    }

    auto trayCardView = _playFieldView->getCardView(trayCard->getCardId());
    if (!trayCardView) {
        trayCardView = _stackView->getCardView(trayCard->getCardId());
    }


    Vec2 playCardTargetPosition = trayCardView->getPosition();
    Vec2 trayCardTargetPosition = trayCardView->getPosition() + Vec2(0, -500);
    playCardView->setClickEnabled(false);
    trayCardView->setClickEnabled(false);

    recordMatchUndo(playCard, trayCard, playCardView->getPosition(), trayCardView->getPosition());

    playCardView->startMoving(playCardTargetPosition);
    trayCardView->startMoving(trayCardTargetPosition);

    _gameModel->setCurrentTrayCard(playCard);
    setCardsEnable(playCard);

    return true;


}
void PlayFieldController::setCardsEnable(CardModel* playCard) {
    for (int id : playCard->getPileUp()._childList) {
        auto card = _gameModel->findCardById(id);
        PileUp pileUp = card->getPileUp();
        pileUp._parentCount--;
        card->setPileUp(pileUp);
        if (pileUp._parentCount <= 0)
            _playFieldView->getCardView(id)->setClickEnabled(true);
    }

}

void PlayFieldController::setCardsDisable(CardModel* playCard) {
    for (int id : playCard->getPileUp()._childList) {
        auto card = _gameModel->findCardById(id);
        PileUp pileUp = card->getPileUp();
        pileUp._parentCount++;
        card->setPileUp(pileUp);
        if (pileUp._parentCount > 0)
            _playFieldView->getCardView(id)->setClickEnabled(false);
    }

}

bool PlayFieldController::performCardMatch(CardModel* playCard, CardModel* trayCard) {
    if (!playCard || !trayCard) {
        return false;
    }

    // 检查两张卡牌是否匹配
    if (playCard->getCardFace() > trayCard->getCardFace()||
        playCard->getCardFace() == (trayCard->getCardFace()-1)) {
        return true;
    }
    else {
        return false;
    }
}


void PlayFieldController::recordMatchUndo(CardModel* playCard, CardModel* originalTrayCard, Vec2 operatedCardPosition, Vec2 originalTrayCardPosition) {
    if (!_undoManager || !playCard || !originalTrayCard) {
        return;
    }

    // 创建回退记录
    UndoRecord undoRecord;
    undoRecord.opType = UNDO_OP_MATCH;
    undoRecord.originalTrayCard = originalTrayCard;
    undoRecord.operatedCard = playCard;
    undoRecord.operatedCardPosition = operatedCardPosition;
    undoRecord.originalTrayCardPosition = originalTrayCardPosition;
    // 添加到回退管理器
    _undoManager->addUndoRecord(undoRecord);
}

void PlayFieldController::undoCardOperation(const UndoRecord& record) {
    if (!_playFieldView || !_gameModel) {
        return;
    }

    if (record.opType != UNDO_OP_MATCH) {
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

    playCardView->startMoving(playCardTargetPosition);
    trayCardView->startMoving(trayCardTargetPosition);
    setCardsDisable(playCard);
}
