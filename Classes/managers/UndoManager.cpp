#include "UndoManager.h"

USING_NS_CC;

UndoManager::UndoManager(GameModel* gameModel, PlayFieldView* playFieldView, StackView* stackView)
    : _undoModel(nullptr)
    , _gameModel(gameModel)
    , _playFieldView(playFieldView)
    , _stackView(stackView) {
}

void UndoManager::init() {
    if (_undoModel) {
        delete _undoModel;
    }
    
    _undoModel = new (std::nothrow) UndoModel();
    if (!_undoModel) {
        return;
    }
    
}

void UndoManager::addUndoRecord(const UndoRecord& record) {
    if (!_undoModel) {
        return;
    }
    
    _undoModel->addUndoRecord(record);
}

bool UndoManager::performUndo() {
    if (!_undoModel) {
        return false;
    }
    
    if (!_undoModel->hasUndoRecords()) {
        return false;
    }
    
    // 获取最新的回退记录
    UndoRecord* record = _undoModel->popUndoRecord();
    if (!record) {
        return false;
    }
    
    // 根据操作类型处理回退
    switch (record->opType) {
        case UNDO_OP_MATCH:
            if(_handleMatchUndo)
            _handleMatchUndo(*record);
            break;
        case UNDO_OP_STACK_DRAW:
            if(_handleDrawUndo)
            _handleDrawUndo(*record);
            break;
      
        default:
            return false;
    }
    
    delete record;
    return true;
}

bool UndoManager::canUndo() const {
    if (!_undoModel) {
        return false;
    }
    
    return _undoModel->hasUndoRecords();
}

void UndoManager::handleMatchUndo(std::function<void(const UndoRecord& record)> handleMatchUndo) {
    this->_handleMatchUndo = handleMatchUndo;
}

void UndoManager::handleDrawUndo(std::function<void(const UndoRecord& record)> handleDrawUndo) {
    this->_handleDrawUndo = handleDrawUndo;
}

