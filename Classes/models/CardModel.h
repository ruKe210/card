#ifndef CARD_MODEL_H
#define CARD_MODEL_H

#include "CardEnums.h"
#include "cocos2d.h"

USING_NS_CC;


class TexturePath {
public:
    std::string _cardFacePathBig;
    std::string _cardFacePathSmall;
    std::string _cardSuitPath;
    std::string _cardBgPath = "res/card_general.png";
};
/**
 * @class PileUp
 * @brief 卡牌堆叠关系类
 * @details 记录卡牌之间的堆叠关系，包括父卡牌数量和子卡牌列表
 */
class PileUp {
public:
    int _parentCount = 0;
    std::vector<int> _childList;
};


class CardModel {
public:
    /**
     * @brief 构造函数
     * @param cardId 卡牌唯一ID
     * @param face 牌面类型
     * @param suit 花色类型
     * @param position 初始位置
     * @param isFlipped 是否翻开
     */
    CardModel(int cardId, CardFaceType face, CardSuitType suit, const Vec2& position);
    
    /**
     * @brief 获取卡牌ID
     * @return 卡牌唯一标识符
     */
    int getCardId() const;
    
    /**
     * @brief 获取牌面类型
     * @return 牌面类型（点数）
     */
    CardFaceType getCardFace() const;
    
    /**
     * @brief 获取花色类型
     * @return 花色类型
     */
    CardSuitType getCardSuit() const;
    
    /**
     * @brief 获取当前位置
     * @return 卡牌在场景中的位置坐标
     */
    Vec2 getPosition() const;
    
    /**
     * @brief 设置位置
     * @param position 新的位置坐标
     */
    void setPosition(const Vec2& position);

    /**
     * @brief 获取卡牌堆叠关系
     * @return 卡牌堆叠关系对象
     */
    PileUp getPileUp();
    
    /**
     * @brief 设置卡牌堆叠关系
     * @param pileUp 堆叠关系对象
     */
    void setPileUp(const PileUp& pileUp);


private:
    int _cardId;               // 卡牌唯一标识符
    CardFaceType _cardFace;    // 牌面类型（点数）
    CardSuitType _cardSuit;    // 花色类型
    Vec2 _position;            // 卡牌在场景中的位置坐标
    PileUp _pileUp;            // 卡牌堆叠关系信息
};

#endif // CARD_MODEL_H