#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<graphics.h>
#include<iostream>
#include<algorithm>
#include<time.h>
#include<functional>
#include<vector>
#include<Windows.h>
#include<string.h>
using namespace std;
#define BORDER_WIDTH 960
#define BORDER_HEIGHT 540

//各种页面的枚举类型
enum PageType {
	menu_page,//主菜单页面
	easy_game_page,//单机-简单模式
	multi_game_page,//双蛇-人机模式or双人模式
	rank_page,//排行榜
};
//游戏难度的枚举类型
enum Level {
	first,//入门版
	second,//进阶版
	third//高级版
};

//封装按钮
class BUTTON {
private:
	pair<int, int>pos;//按钮位置坐标
	wstring text;//按钮文本
	pair<int, int>size;//按钮大小
	float scale;//缩放比例，用于展示鼠标悬停效果
	bool isMouseOver;//鼠标是否处于按钮上方
	void(*onClick)();//发生鼠标点击的时候执行的函数
public:
	//构造函数
	BUTTON(int x, int y, int width, int height, const wstring& info, void(*myFun)());
	//绘制按钮
	void myGUI_draw_BUTTON();
	//检测鼠标是否在按钮上方
	void checkMouseOver(int mouseX, int mouseY);
	//检查鼠标点击是否发生在按钮内并执行后序函数
	bool checkClick(int mouseX, int mouseY);
};

//封装1个页面
class PAGE {
private:
	int page_width;//页面宽度
	int page_height;//页面高度
	IMAGE background;//背景图
	Level level;//游戏难度
	PageType pagetype;//当前页面的类别
	vector<BUTTON>buttons;//当前页面上的所有按钮
	int button_nums;
	ExMessage msg;//本页面的消息
public:
	wstring bkpaper_name;
public:
	//构造函数
	PAGE(int width, int height, PageType pagetype, wstring paper) :page_width(width), page_height(height), pagetype(pagetype), bkpaper_name(paper) {
		level = Level::first;
		msg = { 0 };
		button_nums = 0;
	}
	//初始化当前页面
	void init_page();
	//在当前页面添加按钮信息
	void add_Button();
	//绘制按钮
	void draw();
	//处理鼠标移动
	void mouseMove(int mouseX, int mouseY);
	//处理鼠标点击
	void mouseClick(int mouseX, int mouseY);
	//运行页面
	void run();
};