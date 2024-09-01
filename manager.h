#pragma once
#include"item.h"

//综合管理模块
class MANAGER {
private:
	int time;//每次游戏的计时起点
public:
	SNAKE snake;//管理页面中的所有蛇
	FOOD food;//管理页面中的所有食物
	WALL wall;//管理页面中的所有障碍物
	wchar_t user1[15];//用于输出显示：第一条蛇的用户名
	wchar_t user2[15];//用于输出显示：第二条蛇的用户名
	int winner;//存放每一局的游戏赢家
	bool checkRank;
private:
	void manager_draw_info();//游戏进行中的相关信息
	void manager_readranking();//读取文件中存储的排行榜信息
public:
	MANAGER();//构造函数
	void manager_update();//更新状态
	void manager_ai();//单机游戏下的简易AI模式
	void manager_render();//渲染
	void manager_event(BYTE& vkcode);//事件管理
	void manager_eatfood();//吃食物的相关操作
	void manager_reset_time();//游戏时间重置
	void manager_crashwall();//撞墙的相关操作
	bool manager_isRunning();//检查当局游戏是否还要继续
	void manager_multi_setwinner(int index);//双人模式下设定胜者
	void manager_renderRanking();//打印排行榜
};