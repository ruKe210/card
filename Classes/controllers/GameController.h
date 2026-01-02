#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "cocos2d.h"
#include "Classes/models/GameModel.h"
#include "Classes/views/GameView.h"
#include "Classes/managers/UndoManager.h"
#include "PlayFieldController.h"
#include "StackController.h"

USING_NS_CC;
class PlayFieldController;
class StackController;
/**
 * @class GameController
 * @brief 游戏控制器类
 * @details 协调游戏模型和视图，处理游戏逻辑和用户交互
 *          负责游戏初始化、卡牌点击事件处理、回退操作等核心功能
 *          使用场景：游戏启动、用户操作响应、游戏流程控制
 */
class GameController {
public:
    GameController();
    
    /**
     * @brief 开始游戏
     * @param levelId 关卡ID
     * @return 初始化成功返回true，失败返回false
     */
    bool startGame(int levelId);   
     
    /**
     * @brief 获取游戏视图
     * @return 返回游戏视图对象
     */
    GameView* getGameView() const;
    
    /**
     * @brief 处理回退按钮点击
     */
    void handleUndoClick();
     
    /**
     * @brief 处理主牌区卡牌点击
     * @param cardId 被点击的卡牌ID
     * @return 操作成功返回true，失败返回false
     */
    bool handlePlayFieldCardClick(int cardId);

private:
    GameModel* _gameModel;               // 游戏数据模型，存储游戏状态
    GameView* _gameView;                 // 游戏视图，负责界面显示
    UndoManager* _undoManager;           // 回退管理器，处理操作回退
    PlayFieldController* _playFieldCtrl; // 主牌区控制器，处理主牌区卡牌操作
    StackController* _stackCtrl;         // 备用牌堆控制器，处理抽牌操作

    // 初始化视图回调
    void initViewCallbacks();
};

#endif // GAME_CONTROLLER_H