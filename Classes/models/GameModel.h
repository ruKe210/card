#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include "CardModel.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <vector>
#include <unordered_map>

/**
 * @class GameModel
 * @brief 游戏数据模型类
 * @details 管理整个游戏的核心数据，包括主牌区卡牌、备用牌堆卡牌、当前底牌等
 *          负责游戏状态的维护、卡牌的增删改查以及数据的序列化/反序列化
 *          使用场景：游戏初始化、卡牌操作、游戏存档读取
 */
class GameModel {
public:
    GameModel();
    ~GameModel();
    
    /**
     * @brief 添加主牌区卡牌
     * @param card 要添加的卡牌模型
     */
    void addPlayfieldCard(CardModel* card);
    
    /**
     * @brief 添加备用牌堆卡牌
     * @param card 要添加的卡牌模型
     */
    void addStackCard(CardModel* card);
    
    /**
     * @brief 获取主牌区所有卡牌
     * @return 主牌区卡牌列表
     */
    const std::vector<CardModel*>& getPlayfieldCards() const;
    
    /**
     * @brief 获取备用牌堆卡牌
     * @return 备用牌堆卡牌列表
     */
    const std::vector<CardModel*>& getStackCards() const;
    
    /**
     * @brief 获取当前底牌
     * @return 当前底牌模型，如果没有底牌返回nullptr
     */
    CardModel* getCurrentTrayCard() const;
    
    /**
     * @brief 设置当前底牌
     * @param card 要设置为底牌的卡牌模型
     */
    void setCurrentTrayCard(CardModel* card);
    
    /**
     * @brief 从备用牌堆抽牌
     * @return 抽中的卡牌，失败返回nullptr
     */
    CardModel* drawStackCard();

    /**
     * @brief 查看备用牌堆顶部的卡牌（不弹出）
     * @return 备用牌堆顶部的卡牌，如果没有返回nullptr
     */
    CardModel* peekStackCard();
    
    /**
     * @brief 将卡牌放回备用牌堆
     */
    void pushStackCard();
    
    /**
     * @brief 根据ID查找卡牌
     * @param cardId 卡牌ID
     * @return 找到的卡牌模型，未找到返回nullptr
     */
    CardModel* findCardById(int cardId) const;
    
    /**
     * @brief 反序列化（读档）
     * @param jsonPath JSON文件路径
     */
    void fromJson(const std::string& jsonPath);
 

private:
    std::vector<CardModel*> _playfieldCards;  // 主牌区卡牌列表
    std::vector<CardModel*> _stackCards;     // 备用牌堆卡牌列表
    int _stackTopCard;                        // 备用牌堆顶部卡牌索引
    CardModel* _currentTrayCard;              // 当前底牌（用于匹配的卡牌）
    std::unordered_map<int, CardModel*> _cardIdMap;  // 卡牌ID到卡牌对象的映射（用于快速查找）
};

#endif // GAME_MODEL_H