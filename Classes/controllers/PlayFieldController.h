#ifndef PLAY_FIELD_CONTROLLER_H
#define PLAY_FIELD_CONTROLLER_H

#include "GameController.h"
#include "Classes/models/GameModel.h"
#include "Classes/views/PlayFieldView.h"
#include "Classes/views/StackView.h"
#include "Classes/managers/UndoManager.h"


/**
 * @class PlayFieldController
 * @brief 主牌区控制器类
 * @details 管理主牌区的卡牌操作，包括卡牌点击处理、匹配逻辑、堆叠关系查找
 *          负责主牌区卡牌与底牌的匹配判断和消除操作
 *          使用场景：用户点击主牌区卡牌时的逻辑处理
 */
class PlayFieldController {
public:
    /**
     * @brief 构造函数
     * @param gameModel 游戏数据模型
     * @param playFieldView 主牌区视图
     * @param stackView 备用牌堆视图
     * @param undoManager 回退管理器
     */
    PlayFieldController(GameModel* gameModel, PlayFieldView* playFieldView, StackView* stackView, UndoManager* undoManager);
    
    /**
     * @brief 初始化控制器
     */
    void init();
    
    /**
     * @brief 处理卡牌点击
     * @param cardId 被点击的卡牌ID
     * @return 操作成功返回true，失败返回false
     */
    bool handleCardClick(int cardId);
    
    /**
     * @brief 回退卡牌操作
     * @param record 回退记录
     */
    void undoCardOperation(const UndoRecord& record);

    /**
     * @brief 查找卡牌的堆叠关系
     * @param cardView 卡牌视图
     */
    void findPileUp(CardView* cardView);

    /**
     * @brief 获取主牌区视图
     * @return 主牌区视图对象
     */
    PlayFieldView* getPlayFieldView();

    /**
     * @brief 设置指定卡牌及其子卡牌为可点击状态
     * @param playCard 主牌区卡牌模型
     */
    void setCardsEnable(CardModel* playCard);
    
    /**
     * @brief 设置指定卡牌及其子卡牌为不可点击状态
     * @param playCard 主牌区卡牌模型
     */
    void setCardsDisable(CardModel* playCard);
    
private:
    GameModel* _gameModel;               // 游戏数据模型
    PlayFieldView* _playFieldView;       // 主牌区视图对象
    StackView* _stackView;                // 备用牌堆视图对象
    UndoManager* _undoManager;           // 回退管理器对象
    
    
    // 执行卡牌匹配逻辑
    bool performCardMatch(CardModel* playCard, CardModel* trayCard);
    // 记录匹配操作到回退
    void recordMatchUndo(CardModel* playCard, CardModel* originalTrayCard, Vec2 operatedCardPosition, Vec2 originalTrayCardPosition);


};

#endif // PLAY_FIELD_CONTROLLER_H