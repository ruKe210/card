#ifndef CARD_VIEW_H
#define CARD_VIEW_H

#include "cocos2d.h"
#include "./Classes/models/CardModel.h"

USING_NS_CC;

/**
 * @class CardView
 * @brief 卡牌视图类
 * @details 负责单张卡牌的显示和交互，包括卡牌精灵渲染、点击事件处理、动画效果
 *          继承自Node，支持触摸事件和位置动画
 *          使用场景：卡牌在游戏界面中的显示和交互
 */
class CardView : public Node {
public:
    /**
     * @brief 创建卡牌视图
     * @param cardModel 卡牌数据模型
     * @param clickCallback 点击回调（参数为卡牌ID）
     * @return CardView实例
     */
    static CardView* create(CardModel* cardModel, const std::function<void(int)>& clickCallback);
    
    /**
     * @brief 初始化卡牌视图
     * @param cardModel 卡牌数据模型
     * @param clickCallback 点击回调函数
     * @return 初始化成功返回true，失败返回false
     */
    bool init(CardModel* cardModel, const std::function<void(int)>& clickCallback);
    
    /**
     * @brief 获取对应的卡牌ID
     * @return 卡牌唯一标识符
     */
    int getCardId() const;

    /**
     * @brief 判断触摸点是否在卡牌边界框内（以中心为锚点）
     * @param touchLocation 触摸点位置
     * @param boundingBox 卡牌边界框
     * @return 在边界内返回true，否则返回false
     */
    bool containsPointAnchoredCenter(Vec2 touchLocation, Rect boundingBox);
    
    /**
     * @brief 设置是否可点击
     * @param enabled true表示可点击，false表示不可点击
     */
    void setClickEnabled(bool enabled = true);

    /**
     * @brief 更新卡牌视图（每帧调用）
     * @param dt 时间增量
     */
    void update(float dt) override;

    /**
     * @brief 执行线性插值动画
     * @param lerpSpeed 插值速度
     */
    void lerp(float lerpSpeed);

    /**
     * @brief 开始移动到目标位置
     * @param targetPosition 目标位置坐标
     */
    void startMoving(Vec2 targetPosition);
    
    /**
     * @brief 获取是否正在移动
     * @return 正在移动返回true，否则返回false
     */
    bool getIsMoving();

private:
   
    CardModel* _cardModel;                // 关联的卡牌数据模型
    Sprite* _cardSprite;                  // 卡牌精灵对象，负责卡牌渲染
    std::function<void(int)> _clickCallback; // 点击回调函数，参数为卡牌ID
    bool _clickEnabled;                   // 是否可点击标志
    bool _isOnMoving = false;             // 是否正在移动标志
    Vec2 _targetPosition;                 // 移动目标位置
    float _lerpT = 0 ;                    // 线性插值进度（0.0-1.0）
    // 加载卡牌纹理
    TexturePath getCardTexturePath();
    // 触摸事件处理
    bool onTouchBegan(Touch* touch, Event* event);
};

#endif // CARD_VIEW_H