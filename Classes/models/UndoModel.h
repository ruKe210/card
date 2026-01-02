#ifndef UNDO_MODEL_H
#define UNDO_MODEL_H

#include "CardModel.h"
#include <vector>

// 回退操作类型
enum UndoOpType {
    UNDO_OP_MATCH,    // 匹配消除操作
    UNDO_OP_STACK_DRAW,// 抽备用牌操作
};

// 单条回退记录
struct UndoRecord {
    UndoOpType opType;         // 操作类型
    CardModel* originalTrayCard; // 操作前的底牌
    CardModel* operatedCard;   // 被操作的卡牌（如匹配的主牌）
    Vec2 originalTrayCardPosition;     // 卡牌原始位置（用于回退动画）
    Vec2 operatedCardPosition;     // 卡牌原始位置（用于回退动画）
};

/**
 * @class UndoModel
 * @brief 回退数据模型类
 * @details 维护游戏操作的回退记录栈
 *          负责回退记录的添加、获取和清空
 *          使用场景：存储游戏操作历史，支持回退功能
 */
class UndoModel {
public:
    UndoModel();
    ~UndoModel();
    
    /**
     * @brief 添加回退记录
     * @param record 回退记录
     */
    void addUndoRecord(const UndoRecord& record);
    
    /**
     * @brief 获取最新回退记录（并从列表移除）
     * @return 回退记录指针，如果没有记录返回nullptr
     */
    UndoRecord* popUndoRecord();
    
    /**
     * @brief 是否有可回退记录
     * @return 有可回退记录返回true，否则返回false
     */
    bool hasUndoRecords() const;
    
    /**
     * @brief 清空所有回退记录
     */
    void clearUndoRecords();

private:
    std::vector<UndoRecord*> _undoRecords; // 回退记录栈，存储所有可回退的操作记录
};

#endif // UNDO_MODEL_H