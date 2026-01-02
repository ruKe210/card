#ifndef STACK_CONTROLLER_H
#define STACK_CONTROLLER_H

#include "Classes/models/GameModel.h"
#include "Classes/views/StackView.h"
#include "Classes/views/PlayFieldView.h"
#include "Classes/managers/UndoManager.h"

/**
 * @class StackController
 * @brief 备用牌堆控制器类
 * @details 管理备用牌堆的操作，包括抽牌、替换底牌等
 *          负责备用牌堆的点击事件处理和抽牌逻辑
 *          使用场景：用户点击备用牌堆时的抽牌操作
 */
class StackController {
public:
    /**
     * @brief 构造函数
     * @param gameModel 游戏数据模型
     * @param playFieldView 主牌区视图
     * @param stackView 备用牌堆视图
     * @param undoManager 回退管理器
     */
    StackController( GameModel* gameModel, PlayFieldView* playFieldView, StackView* stackView, UndoManager* undoManager);
    
    /**
     * @brief 初始化控制器
     */
    void init();
    
    /**
     * @brief 处理备用牌堆点击（抽牌）
     * @return 操作成功返回true，失败返回false
     */
    bool handleStackClick();
    
    /**
     * @brief 回退抽牌/替换操作
     * @param record 回退记录
     */
    void undoStackOperation(const UndoRecord& record);
    
    /**
     * @brief 获取备用牌堆视图
     * @return 备用牌堆视图对象
     */
    StackView* getStackView();


private:
    GameModel* _gameModel;               // 游戏数据模型
    PlayFieldView* _playFieldView;       // 主牌区视图对象
    StackView* _stackView;               // 备用牌堆视图对象
    UndoManager* _undoManager;           // 回退管理器对象
    
    // 记录抽牌操作到回退
    void recordDrawUndo(CardModel* playCard, CardModel* originalTrayCard, Vec2 operatedCardPosition, Vec2 originalTrayCardPosition);
};

#endif // STACK_CONTROLLER_H