#ifndef UNDO_MANAGER_H
#define UNDO_MANAGER_H

#include "Classes/models/UndoModel.h"
#include "Classes/models/GameModel.h"
#include "Classes/Views/PlayFieldView.h"
#include "Classes/Views/StackView.h"

/**
 * @class UndoManager
 * @brief 回退管理器类
 * @details 管理游戏操作的回退功能，维护回退记录栈
 *          支持匹配操作和抽牌操作的回退，协调模型和视图的回退逻辑
 *          使用场景：用户点击回退按钮时恢复上一步操作
 */
class UndoManager {
public:
    /**
     * @brief 构造函数
     * @param gameModel 游戏数据模型
     * @param playFieldView 主牌区视图
     * @param stackView 备用牌堆视图
     */
    UndoManager(GameModel* gameModel, PlayFieldView* playFieldView, StackView* stackView);
    
    /**
     * @brief 初始化管理器
     */
    void init();
    
    /**
     * @brief 添加回退记录
     * @param record 回退记录
     */
    void addUndoRecord(const UndoRecord& record);
    
    /**
     * @brief 执行回退操作
     * @return 回退成功返回true，失败返回false
     */
    bool performUndo();
    
    /**
     * @brief 是否有可回退记录
     * @return 有可回退记录返回true，否则返回false
     */
    bool canUndo() const;

    /**
     * @brief 设置匹配操作回退回调函数
     * @param handleMatchUndo 匹配操作回退处理函数
     */
    void handleMatchUndo(std::function<void(const UndoRecord& record)> handleMatchUndo);
    
    /**
     * @brief 设置抽牌操作回退回调函数
     * @param handleDrawUndo 抽牌操作回退处理函数
     */
    void handleDrawUndo(std::function<void(const UndoRecord& record)> handleDrawUndo);

private:
    UndoModel* _undoModel;               // 回退数据模型对象
    GameModel* _gameModel;               // 游戏数据模型对象
    PlayFieldView* _playFieldView;       // 主牌区视图对象
    StackView* _stackView;               // 备用牌堆视图对象
    std::function<void(const UndoRecord& record)> _handleMatchUndo; // 匹配操作回退回调函数
    std::function<void(const UndoRecord& record)> _handleDrawUndo; // 抽牌操作回退回调函数

};

#endif // UNDO_MANAGER_H