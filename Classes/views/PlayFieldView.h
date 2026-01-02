#ifndef PLAY_FIELD_VIEW_H
#define PLAY_FIELD_VIEW_H

#include "cocos2d.h"
#include "CardView.h"
#include <unordered_map>

USING_NS_CC;

/**
 * @class PlayFieldView
 * @brief 主牌区视图类
 * @details 管理主牌区所有卡牌的显示和交互
 *          负责卡牌视图的添加、移除和查找，处理卡牌点击事件
 *          使用场景：显示和管理主牌区的卡牌
 */
class PlayFieldView : public Node {
public:
    /**
     * @brief 创建主牌区视图
     * @param cardClickCallback 卡牌点击回调函数
     * @return PlayFieldView实例
     */
    static PlayFieldView* create(const std::function<void(int)>& cardClickCallback);
    
    /**
     * @brief 初始化主牌区视图
     * @param cardClickCallback 卡牌点击回调函数
     * @return 初始化成功返回true，失败返回false
     */
    bool init(const std::function<void(int)>& cardClickCallback) ;
    
    /**
     * @brief 添加卡牌视图
     * @param cardView 要添加的卡牌视图
     */
    void addCardView(CardView* cardView);
    
    /**
     * @brief 移除卡牌视图
     * @param cardId 要移除的卡牌ID
     */
    void removeCardView(int cardId);
    
    /**
     * @brief 获取卡牌视图
     * @param cardId 卡牌ID
     * @return 卡牌视图对象，未找到返回nullptr
     */
    CardView* getCardView(int cardId) const;

private:
    std::unordered_map<int, CardView*> _cardViewMap; // 卡牌ID到卡牌视图的映射表
    std::function<void(int)> _cardClickCallback;     // 卡牌点击回调函数，参数为卡牌ID
};

#endif // PLAY_FIELD_VIEW_H