#pragma once
#include"myGUI.h"

const COLORREF skin_color[3] = { RGB(238,92,66),RGB(255,165,0),RGB(0,154,205) };

typedef struct pos {
	int x;//位置X坐标
	int y;//位置Y坐标
	COLORREF color;//当前位置的颜色
}pos;
//枚举出4种可能的方向
enum DIR {
	up, down, left, right
};
enum MODE {
	easy,//单机模式：页面上只有一条蛇
	multi//双机模式：页面上有2条蛇，包括人机对战和双人模式
};


class SNAKE {
private:
	void snake_invalid(int index);
public:
	Level level;//当前小蛇所属的游戏难度，决定具体的功能设定
	MODE mode;//决定页面上是一条蛇还是两条蛇
	int color[2];//小蛇的皮肤颜色
	int length[2];//蛇的长度
	enum DIR direction[2];//蛇的方向
	int radio;//蛇身的半径
	int speed[2];//蛇的前进速度
	vector<vector<pos>>snake_pos;//每一节蛇身的坐标
	int score[2];//这条蛇当前得到的总分
	int life[2];//生命值
	int crashtimes[2];//撞墙次数
	bool cgtoAI;//简单模式下是否切换AI模式

public:
	SNAKE();//构造函数
	void snake_eyeRender(int x, int y);//蛇的眼睛绘制
	void snake_bodyRender(int index);//蛇的身体绘制
	void set_skincolor(int index, int color);//重置皮肤颜色
	void snake_move(int index);//蛇的移动
	void snake_event(int inedx, BYTE bkcode);//蛇的事件处理
	void snake_increase(int index, int increase);//蛇的长度增加
	void snake_decrease(int index, int decrease);//蛇的长度缩短
	void way_to_exit();//不同游戏模式下的退出方式
	void snake_reset(int index);//重置蛇的各项属性
	bool snake_checklife(int index);//检查蛇的存活状态
	void snake_easyAI(int index, pos target);//单条蛇的AI模式算法
};

class WALL;

class FOOD {
private:
	bool good_bad[50];//设置是好果子还是坏果子
	int score[50];
	bool alive[50];//果子是否被吃掉
public:
	COLORREF color[50];//食物的颜色：好果子-彩色，坏果子-灰色
	int sum;//每一次的果子总数
	int radio;//绘制半径
	pos food_pos[50];//食物的坐标
	int left;//剩余的还没被吃掉的果子
public:
	FOOD() {};//默认构造函数
	void food_set(SNAKE& snake_group, WALL& wall);//设置所有果子的属性（构造函数调用，重置函数就是set本身）
	void food_render();//绘制渲染
	bool food_getAlive(int index);//判断食物存活状态
	void food_setAlive(int index, bool setAlive);//设置食物存活状态
	int food_getScore(int index);//获取食物对应分数
	bool food_getQuality(int index);//获取食物好坏性质
	bool food_checkpos(int food_index);//检查当前生成的食物是否与已有食物重合
	void food_Cgbysnake(vector<pos>& snake_pos);//在撞墙后的蛇的尾部生成随机食物
};



//障碍物类
class WALL {
private:
	int wall_num;//墙壁总数
	int wall_radio;
	vector<bool>wall_soft_hard;//软墙壁和硬墙壁
	vector<COLORREF>wall_color;//软墙壁和硬墙壁
public:
	vector<pos>wall_pos;//除了边框以外，所有障碍物的位置
public:
	WALL();//构造函数
	void wall_build(vector<pos>dead_snake_pos);//把死掉的蛇蛇变成墙壁
	void wall_render();//绘制所有墙壁
	bool wall_getQuality(int wall_index);//获取当前墙壁的属性
	bool wall_checkpos(int food_x, int food_y);//检查新生成的食物是否出现在了墙壁上
	void wall_reset();//重置墙壁属性
};
