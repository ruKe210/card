#ifndef STACK_VIEW_H
#define STACK_VIEW_H

#include "cocos2d.h"
#include "CardView.h"

USING_NS_CC;

/**
 * @class StackView
 * @brief 备用牌堆视图类
 * @details 管理备用牌堆和底牌的显示
 *          负责备用牌堆精灵、底牌视图、剩余牌数标签的显示和管理
 *          使用场景：显示备用牌堆和当前底牌
 */
class StackView : public Node {
public:
    /**
     * @brief 创建备用牌堆视图
     * @param stackClickCallback 备用牌堆点击回调函数
     * @return StackView实例
     */
    static StackView* create(const std::function<void()>& stackClickCallback);
    
    /**
     * @brief 初始化备用牌堆视图
     * @param stackClickCallback 备用牌堆点击回调函数
     * @return 初始化成功返回true，失败返回false
     */
    bool init(const std::function<void()>& stackClickCallback) ;
    
    /**
     * @brief 设置底牌视图
     * @param cardView 要设置为底牌的卡牌视图
     */
    void setTrayCardView(CardView* cardView);

    /**
     * @brief 获取卡牌视图
     * @param cardId 卡牌ID
     * @return 卡牌视图对象，未找到返回nullptr
     */
    CardView* getCardView(int cardId) const;

    /**
     * @brief 添加卡牌视图
     * @param cardView 要添加的卡牌视图
     */
    void addCardView(CardView* cardView);
    
    /**
     * @brief 获取当前底牌视图
     * @return 当前底牌视图对象，如果没有底牌返回nullptr
     */
    CardView* getCurrentTrayCardView() const;



private:
    std::unordered_map<int, CardView*> _cardViewMap; // 卡牌ID到卡牌视图的映射表
    CardView* _currentTrayCardView;    // 当前底牌视图对象
    Sprite* _stackSprite;              // 备用牌堆精灵对象
    Label* _stackCountLabel;           // 剩余牌数标签对象
    std::function<void()> _stackClickCallback; // 备用牌堆点击回调函数
};

#endif // STACK_VIEW_H