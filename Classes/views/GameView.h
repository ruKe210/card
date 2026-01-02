#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "cocos2d.h"
#include "PlayFieldView.h"
#include "StackView.h"
USING_NS_CC;

/**
 * @class GameView
 * @brief 游戏主视图类
 * @brief 游戏主视图类
 * @details 游戏界面的顶层容器，包含主牌区视图和备用牌堆视图
 *          负责整体布局、触摸事件分发和回退按钮管理
 *          使用场景：游戏主界面的显示和交互
 */
class GameView : public Node {
public:
    /**
     * @brief 创建游戏视图
     * @return GameView实例
     */
    static GameView* create();
    
    /**
     * @brief 初始化游戏视图
     * @return 初始化成功返回true，失败返回false
     */
    bool init() override;
    
    /**
     * @brief 获取主牌区视图
     * @return 主牌区视图对象
     */
    PlayFieldView* getPlayFieldView() const;
    
    /**
     * @brief 获取备用牌堆+底牌视图
     * @return 备用牌堆视图对象
     */
    StackView* getStackView() const;

    /**
     * @brief 触摸开始事件处理
     * @param touch 触摸对象
     * @param event 事件对象
     * @return 事件被处理返回true，否则返回false
     */
    bool onTouchBegan(Touch* touch, Event* event);

    /**
     * @brief 设置回退按钮点击回调
     * @param clickCallback 点击回调函数
     */
    void setUndoBtnCallback(std::function<void()> clickCallback);

private:
    PlayFieldView* _playFieldView; // 主牌区视图对象
    StackView* _stackView;         // 备用牌堆和底牌视图对象
    Label* _undoBtn;              // 回退按钮标签对象
    std::function<void()> _clickCallback; // 回退按钮点击回调函数
};

#endif // GAME_VIEW_H