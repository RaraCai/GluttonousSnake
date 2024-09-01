#include"manager.h"
#include"Record.h"

extern MANAGER manager;
const COLORREF good_food_color[5] = { RGB(255, 127, 0) ,RGB(154,205,50),RGB(205,96,144),RGB(173,216,230),RGB(255,215,0) };
#define GAME_BEGIN_X 46
#define GAME_BEGIN_Y 93
#define GAME_WIDTH 415
#define GAME_HEIGHT 398
#define GAME_MULTI_BEGIN_X 49
#define GAME_MULTI_BEGIN_Y 145
#define GAME_MULTI_WIDTH 860
#define GAME_MULTI_HEIGHT 353


//菜单函数声明
void myMenu();
//写入文件记录函数声明
void write_record(PageType pagetype, MODE mode, Level level, char user1[], int score1, int score2 = 0, char user2[] = (char*)"AI_snake");

//简易模式下退出游戏页面
void easy_exit() {
	HWND h = GetHWnd();
	if (MessageBox(h, _T("游戏结束!\n按确定键返回菜单"), _T("游戏提示"), MB_OK)) {
		//这里需要更新一下文件中的历史记录
		char user1[15] = { 0 };
		sprintf_s(user1, "%ws", manager.user1);
		write_record(PageType::easy_game_page, manager.snake.mode, manager.snake.level, user1, manager.snake.score[0]);
		//更新排行榜
		easy_num++;
		update_rank(manager.snake.score[0], user1, manager.snake.level, 1);
		write_rank();
		//简单状态下直接重置当前蛇的各项属性为初始状态，不保存当前地图
		manager.food.food_set(manager.snake, manager.wall);
		manager.snake.snake_reset(0);
		manager.wall.wall_reset();
		myMenu();
	}

}
//两个蛇的模式的退出
void multi_exit() {
	HWND h = GetHWnd();
	if (manager.snake.cgtoAI == false) {//双人模式
		if (manager.winner == 1)//player1 win
			MessageBox(h, _T("本局赢家：Player1"), _T("游戏提示"), MB_OK);
		else if (manager.winner == 2)//player2 win
			MessageBox(h, _T("本局赢家：Player2"), _T("游戏提示"), MB_OK);
		else //win equal
			MessageBox(h, _T("本局平局！"), _T("游戏提示"), MB_OK);
		multi_num += 2;
		//multi_max = max(manager.snake.score[0], manager.snake.score[1]);
	}
	else {//人机对战模式
		if (manager.winner == 1)//player1 win
			MessageBox(h, _T("本局赢家：Player"), _T("游戏提示"), MB_OK);
		else if (manager.winner == 2)//AI player2 win
			MessageBox(h, _T("本局赢家：AI-Player"), _T("游戏提示"), MB_OK);
		else {//win equal
			MessageBox(h, _T("本局平局！"), _T("游戏提示"), MB_OK);
			ai_num++;
		}
		ai_num++;
	}

	//这里需要更新一下文件中的历史记录
	char user1[15] = { 0 };
	char user2[15] = { 0 };
	//sprintf_s(user1, "%ws", manager.user1);
	//sprintf_s(user2, "%ws", manager.user2);

	WideCharToMultiByte(CP_ACP, 0, manager.user1, -1, user1, 15, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, manager.user2, -1, user2, 15, NULL, NULL);

	write_record(PageType::easy_game_page, manager.snake.mode, manager.snake.level, user1, manager.snake.score[0], manager.snake.score[1], user2);

	//更新排行榜
	//双人模式和人机模式都同时更新两个玩家的游戏记录，包括AI蛇的记录
	update_rank(manager.snake.score[0], user1, manager.snake.level, 3 - manager.snake.cgtoAI);
	update_rank(manager.snake.score[1], user2, manager.snake.level, 3 - manager.snake.cgtoAI);
	write_rank();//写入排行榜文件

	//简单状态下直接重置当前蛇的各项属性为初始状态，不保存当前地图
	manager.food.food_set(manager.snake, manager.wall);
	manager.wall.wall_reset();
	manager.snake.snake_reset(0);
	manager.snake.snake_reset(1);//双人对战第二条蛇也需要重置
	//返回主菜单
	myMenu();

}

//单机模式和双机模式下的同退出情况
void SNAKE::way_to_exit() {
	if (this->mode == MODE::easy) {
		easy_exit();
	}
	else if (this->mode == MODE::multi) {
		multi_exit();
	}
}
//3个版本下蛇撞到边界时的不同处理
void SNAKE::snake_invalid(int index) {
	bool ai = this->cgtoAI;
	bool notset = (this->mode == MODE::multi) && ai;
	int score_before = 0;
	switch (this->level) {
	case Level::first:
		manager.manager_multi_setwinner(index + 1);
		way_to_exit();
		break;
	case Level::second:
		manager.wall.wall_build(this->snake_pos[index]);
		score_before = manager.snake.score[index];
		manager.snake.snake_reset(index);
		manager.snake.score[index] = score_before;
		if (notset)
			this->cgtoAI = ai;
		break;
	case Level::third:
		manager.food.food_Cgbysnake(manager.snake.snake_pos[index]);
		score_before = manager.snake.score[index];
		manager.snake.snake_reset(index);
		manager.snake.score[index] = score_before;
		if (notset)
			this->cgtoAI = ai;
		break;
	}
}



//初始化蛇
SNAKE::SNAKE() {
	this->cgtoAI = false;
	//先初始化存放各个蛇头的坐标二维的vector
	snake_pos.resize(2);
	this->radio = 5;
	//2条蛇的默认颜色
	this->color[0] = 0; color[1] = 2;
	this->set_skincolor(0, color[0]);
	this->set_skincolor(1, color[1]);
	for (int index = 0; index < 2; index++) {
		this->direction[index] = DIR::right;//初始方向right
		this->length[index] = 3;//初始长度为3
		this->score[index] = 0;//初始得分
		this->speed[index] = 1;
		this->life[index] = 10;//一节身体（不算蛇头）5个生命值
		//初始蛇头位置放入vector
		pos begin = { 300 + index * 100,300 + index * 100,skin_color[this->color[index]] };
		snake_pos[index].push_back(begin);


		//初始化各个身体节的颜色
		for (int i = 1; i < this->length[index]; i++) {
			pos section = { 0 };
			if (i % 2)
				section.color = WHITE;
			else
				section.color = skin_color[this->color[index]];

			section.x = snake_pos[index][i - 1].x - radio;
			section.y = snake_pos[index][i - 1].y;
			snake_pos[index].push_back(section);
		}
	}

	crashtimes[0] = 0, crashtimes[1] = 0;
}
//渲染蛇的眼睛
void SNAKE::snake_eyeRender(int x, int y) {
	setfillcolor(WHITE);
	solidcircle(x, y, 3);
	setfillcolor(BLACK);
	solidcircle(x, y, 2);
	setfillcolor(WHITE);
	solidcircle(x + 1, y - 1, 1);
}
//渲染蛇的身体
void SNAKE::snake_bodyRender(int index) {

	for (int i = this->length[index] - 1; i >= 0; i--) {
		setfillcolor(this->snake_pos[index][i].color);
		solidcircle(snake_pos[index][i].x, snake_pos[index][i].y, this->radio);
	}
	//画眼睛，针对当前蛇头朝向作分类讨论
	int eyewidth = 4;//两眼之间的宽度
	switch (this->direction[index]) {
	case DIR::up:
	case DIR::down:
		snake_eyeRender(snake_pos[index][0].x - eyewidth, snake_pos[index][0].y);
		snake_eyeRender(snake_pos[index][0].x + eyewidth, snake_pos[index][0].y);
		break;
	case DIR::left:
	case DIR::right:
		snake_eyeRender(snake_pos[index][0].x, snake_pos[index][0].y - eyewidth);
		snake_eyeRender(snake_pos[index][0].x, snake_pos[index][0].y + eyewidth);
		break;
	}
}
//更改蛇的皮肤颜色
void SNAKE::set_skincolor(int index, int color) {
	this->color[index] = color;
	for (int i = 0; i < this->length[index]; i++) {
		if (i % 2 == 0)
			this->snake_pos[index][i].color = skin_color[color];
	}
}
//蛇的移动模块
void SNAKE::snake_move(int index) {

	int up_limit = 0, down_limit = 0, left_limit = 0, right_limit = 0;
	switch (this->mode) {
	case MODE::easy:
		up_limit = GAME_BEGIN_Y;
		down_limit = GAME_BEGIN_Y + GAME_HEIGHT;
		left_limit = GAME_BEGIN_X;
		right_limit = GAME_BEGIN_X + GAME_WIDTH;
		break;
	case MODE::multi:
		up_limit = GAME_MULTI_BEGIN_Y;
		down_limit = GAME_MULTI_BEGIN_Y + GAME_MULTI_HEIGHT;
		left_limit = GAME_MULTI_BEGIN_X;
		right_limit = GAME_MULTI_BEGIN_X + GAME_MULTI_WIDTH;
		break;
	}


	//后一个的位置是前一个在上次的位置
	for (int i = this->length[index] - 1; i > 0; i--) {
		snake_pos[index][i].x = snake_pos[index][i - 1].x;
		snake_pos[index][i].y = snake_pos[index][i - 1].y;
	}
	//根据方向做移动
	switch (this->direction[index]) {
	case DIR::up:
		snake_pos[index][0].y -= this->speed[index];
		if (snake_pos[index][0].y <= up_limit)
		{/*这里依据游戏模式作特判*//*直接死亡还是变成食物和墙*/
			snake_invalid(index);
			crashtimes[index]++;
		}
		break;
	case DIR::down:
		snake_pos[index][0].y += this->speed[index];
		if (snake_pos[index][0].y >= down_limit)
		{/*这里依据游戏模式作特判*//*直接死亡还是变成食物和墙*/
			snake_invalid(index);
			crashtimes[index]++;
		}
		break;
	case DIR::left:
		snake_pos[index][0].x -= this->speed[index];
		if (snake_pos[index][0].x - this->radio <= left_limit)
		{/*这里依据游戏模式作特判*//*直接死亡还是变成食物和墙*/
			snake_invalid(index);
			crashtimes[index]++;
		}
		break;
	case DIR::right:
		snake_pos[index][0].x += this->speed[index];
		if (snake_pos[index][0].x + this->radio >= right_limit)
		{/*这里依据游戏模式作特判*//*直接死亡还是变成食物和墙*/
			snake_invalid(index);
			crashtimes[index]++;
		}
		break;
	}
}
//蛇的事件
void SNAKE::snake_event(int index, BYTE vkcode) {
	if (index == 0) {
		switch (vkcode) {
		case VK_LEFT:
			if (this->direction[index] != DIR::right) {
				this->direction[index] = DIR::left;
			}
			break;
		case VK_RIGHT:
			if (this->direction[index] != DIR::left)
				this->direction[index] = DIR::right;
			break;
		case VK_UP:
			if (this->direction[index] != DIR::down)
				this->direction[index] = DIR::up;
			break;
		case VK_DOWN:
			if (this->direction[index] != DIR::up)
				this->direction[index] = DIR::down;
			break;
		case VK_SPACE:
			if (this->speed[index] == 1)//加速处理
				this->speed[index] = 3;
			else if (this->speed[index] == 3)
				this->speed[index] = 1;//已经处于加速状态，再次按空格还原回初始速度
			break;
		}
	}
	else if (index == 1) {
		switch (vkcode) {
		case 'a':
		case 'A':
			if (this->direction[index] != DIR::right) {
				this->direction[index] = DIR::left;
			}
			break;
		case 'd':
		case 'D':
			if (this->direction[index] != DIR::left)
				this->direction[index] = DIR::right;
			break;
		case 'w':
		case 'W':
			if (this->direction[index] != DIR::down)
				this->direction[index] = DIR::up;
			break;
		case 's':
		case 'S':
			if (this->direction[index] != DIR::up)
				this->direction[index] = DIR::down;
			break;
		case 'z':
		case 'Z':
			if (this->speed[index] == 1)//加速处理
				this->speed[index] = 3;
			else if (this->speed[index] == 3)
				this->speed[index] = 1;//已经处于加速状态，再次按空格还原回初始速度
			break;
		}
	}
}
//蛇的增长
void SNAKE::snake_increase(int index, int increase) {
	////新增证明得分，游戏提示音
	//mciSendString(_T("close incorrect"), NULL, 0, NULL);
	//mciSendString(_T("open ./increase.mp3 alias incorrect"), NULL, 0, NULL);
	//mciSendString(_T("play incorrect"), NULL, 0, NULL);
	//对新增节数的颜色作初始化
	for (int i = this->length[index]; i < length[index] + increase; i++) {
		pos incre_section = { 0 };
		if (i % 2 == 0)
			incre_section.color = skin_color[this->color[index]];
		else
			incre_section.color = WHITE;
		this->snake_pos[index].push_back(incre_section);
	}
	//改变蛇身长度
	this->length[index] += increase;// 吃好果子得分
	//改变当前蛇的得分
	this->score[index] += increase * 10;
	//改变当前蛇的生命数
	this->life[index] += increase * 5;
}
//蛇的减少
void SNAKE::snake_decrease(int index, int decrease) {
	//判断减少的长度是否合法
	if (decrease >= this->length[index]) {
		this->life[index] = 0;
		this->score[index] -= decrease * 10;
		return;
	}
	//若长度减少后蛇还能存活，则从vector中最后一个元素开始按长度减少
	for (int i = 0; i < decrease; i++)
		this->snake_pos[index].pop_back();
	this->length[index] -= decrease;
	this->life[index] -= decrease * 5;//吃坏果子不得分 减生命
}
//重置蛇的各项属性
void SNAKE::snake_reset(int index) {
	//重置两条蛇颜色
	this->color[0] = 0; color[1] = 2;
	this->set_skincolor(0, color[0]);
	this->set_skincolor(1, color[1]);
	//得分修改为0
	this->score[index] = 0;
	snake_pos.resize(2);
	//清空所有元素，再设置回初始长度
	this->snake_pos[index].clear();
	this->length[index] = 3;//蛇身长度设回初始长度=3
	//重置蛇头位置及其后两节的位置
	//初始蛇头位置放入vector
	pos begin = { 300 + index * 100,300 + index * 100,skin_color[this->color[index]] };
	snake_pos[index].push_back(begin);
	//初始化各个身体节的颜色
	for (int i = 1; i < this->length[index]; i++) {
		pos section = { 0 };
		if (i % 2)
			section.color = WHITE;
		else
			section.color = skin_color[this->color[index]];

		section.x = snake_pos[index][i - 1].x - radio;
		section.y = snake_pos[index][i - 1].y;
		snake_pos[index].push_back(section);
	}
	//蛇的速度修改回初始速度
	this->speed[index] = 1;
	//是否进入AI模式修改回false
	this->cgtoAI = false;
	//蛇的生命值修改回初始状态
	this->life[index] = 10;
}
//检查蛇的死活状态
bool SNAKE::snake_checklife(int index) {
	//蛇的生命值降为0及以下，third模式下撞墙超5次，头尾相接：游戏结束的判定条件
	if (this->life[index] <= 0 || (level == Level::third && crashtimes[index] > 5) ||
		(snake_pos[index][0].x == snake_pos[index][length[index] - 1].x && snake_pos[index][0].y == snake_pos[index][length[index] - 1].y))
	{
		//设置本局游戏的胜利者
		manager.manager_multi_setwinner(index + 1);
		////分数清空
		//manager.snake.score[index] = 0;
		if (level == Level::third && crashtimes[index] > 5) {
			HWND h = GetHWnd();
			MessageBox(h, _T("Sorry!!在当前模式下撞墙超过5次就会结束游戏哦~"), _T("游戏提示"), MB_OK);
		}
		return false;//蛇到了该死的时候就返回false
	}
	else if (mode == MODE::multi && abs(score[0] - score[1]) > 200) {
		//设置游戏胜者
		int winner = (score[0] > score[1]) ? 1 : 0;
		manager.manager_multi_setwinner(winner + 1);
		return false;
	}
	else
		return true;//小蛇还活着
}
//简单模式下的AI蛇
void SNAKE::snake_easyAI(int index, pos target) {
	//获取当前的蛇头坐标
	int head_x = this->snake_pos[index][0].x;
	int head_y = this->snake_pos[index][0].y;

	if (abs(head_x - target.x) > 3 && abs(head_y - target.y) > 3)
		speed[index] = 3;
	else if (head_x == target.x && abs(head_y - target.y) > 3)
		speed[index] = 3;
	else if (head_y == target.y && abs(head_x - target.x) > 3)
		speed[index] = 3;
	else
		speed[index] = 1;

	//将蛇头坐标与目标坐标比较
	if (head_x < target.x) {
		if (direction[index] != DIR::left)
			direction[index] = DIR::right;
		else {
			if (target.y > head_y)
				direction[index] = DIR::down;
			else if (target.y < head_y)
				direction[index] = DIR::up;
		}
	}
	else if (head_x > target.x) {
		if (direction[index] != DIR::right)
			direction[index] = DIR::left;
		else {
			if (target.y > head_y)
				direction[index] = DIR::down;
			else if (target.y < head_y)
				direction[index] = DIR::up;
		}
	}
	else if (head_y < target.y) {
		if (direction[index] != DIR::up)
			direction[index] = DIR::down;
		else {
			if (target.x > head_x)
				direction[index] = DIR::right;
			else if (target.x < head_x)
				direction[index] = DIR::left;
		}
	}
	else if (head_y > target.y) {
		if (direction[index] != DIR::down)
			direction[index] = DIR::up;
		else {
			if (target.x > head_x)
				direction[index] = DIR::right;
			else if (target.x < head_x)
				direction[index] = DIR::left;
		}
	}
}




/*******************************FOOD类成员函数的实现*************************************/

//判读食物当前状态
bool FOOD::food_getAlive(int index) {
	return this->alive[index];//还没被吃掉返回true
}
//绘制食物
void FOOD::food_render() {
	for (int i = 0; i < this->sum; i++) {
		if (this->alive[i] == true) {
			setfillcolor(this->color[i]);
			solidcircle(food_pos[i].x, food_pos[i].y, this->radio);
			setfillcolor(WHITE);
			solidcircle(food_pos[i].x - 2, food_pos[i].y - 2, 2);
		}
	}
}
//初始化或者重置当前页面所有果子的状态
void FOOD::food_set(SNAKE& snake_group, WALL& wall) {

	int begin_x = (manager.snake.mode == MODE::easy) ? GAME_BEGIN_X : GAME_MULTI_BEGIN_X;
	int begin_y = (manager.snake.mode == MODE::easy) ? GAME_BEGIN_Y : GAME_MULTI_BEGIN_Y;
	int width = (manager.snake.mode == MODE::easy) ? GAME_WIDTH : GAME_MULTI_WIDTH;
	int height = (manager.snake.mode == MODE::easy) ? GAME_HEIGHT : GAME_MULTI_HEIGHT;

	//先将所有果子的alive属性都设为false
	for (int i = 0; i < 50; i++)
		this->alive[i] = false;
	//首先随机生成初始时的果子总数
	this->sum = rand() % 5 + 1;
	this->left = 0;//好果子的初始数量为0，根据后面生成的果子属性计数
	//依次设置果子的属性
	for (int i = 0; i < this->sum; i++) {
		//设置所有果子的属性都为没被吃掉
		this->alive[i] = true;
		//随机生成果子的分数
		this->score[i] = rand() % 3 + 5;
		//随机生成这些果子的好坏属性
		this->good_bad[i] = rand() % 2;//1-true-good,0-false-bad
		//若是生成好果子，则计数量加1
		if (good_bad[i])
			this->left++;
		//并依据好坏属性设置果子的颜色
		if (good_bad[i] == true)//good
			this->color[i] = good_food_color[rand() % 5];
		else//bad
			this->color[i] = RGB(139, 137, 112);
		//随机生成当前果子的位置坐标
		bool valid = true;
		while (true) {
			valid = true;
			food_pos[i].x = rand() % (width - 40) + begin_x + 20;
			food_pos[i].y = rand() % (height - 40) + begin_y + 20;
			//若生成的位置与任意一个已经有的食物重合，也需要重新生成
			while (this->food_checkpos(i) == false)
			{
				food_pos[i].x = rand() % (width - 40) + begin_x + 20;
				food_pos[i].y = rand() % (height - 40) + begin_y + 20;

			}

			//确保新生成的果子不在任何一个墙上
			while (wall.wall_checkpos(food_pos[i].x, food_pos[i].y) == false) {
				food_pos[i].x = rand() % (width - 40) + begin_x + 20;
				food_pos[i].y = rand() % (height - 40) + begin_y + 20;
			}

			//确保新生成的果子位置不在任何一条蛇的身上
			int snake_index = 0;
			int index = 0;
			for (snake_index = 0; snake_index <= (int)snake_group.mode; snake_index++) {
				for (index = 0; index < snake_group.length[snake_index]; index++) {
					if (food_pos[i].x == snake_group.snake_pos[snake_index][index].x ||
						food_pos[i].y == snake_group.snake_pos[snake_index][index].y) {
						valid = false;
						break;
					}
				}
				if (valid == false)
					break;
			}
			if (valid == false)
				continue;
			else
				break;
		}//生成了正确的食物坐标
	}
}
//设置某一个食物的存活状态
void FOOD::food_setAlive(int index, bool setAlive) {
	this->alive[index] = setAlive;
}
//获取某一个食物的分数
int FOOD::food_getScore(int index) {
	return this->score[index];
}
//判断某一个食物的好坏质量
bool FOOD::food_getQuality(int index) {
	return this->good_bad[index];//好果子返回true
}
//判断当前生成的这个位置会不会与已有的食物重叠。若不重叠说明位置有效返回true
bool FOOD::food_checkpos(int food_index) {
	for (int i = 0; i < food_index; i++) {
		if (this->food_pos[food_index].x == food_pos[i].x && this->food_pos[food_index].y == food_pos[i].y)
			return false;
	}
	return true;
}
//在撞墙的蛇的尾部随机生成食物
void FOOD::food_Cgbysnake(vector<pos>& snake_pos) {
	if (this->sum >= 50)
		return;
	//蛇在撞墙的位置随机变成1~3个食物
	int length = snake_pos.size();
	int newpos[3] = { -1,-1,-1 };
	//若长度小于3个则默认变成墙
	if (length > 3) {
		//在蛇的身上找3个随机位置
		for (int i = 0; i < 3; i++) {
			newpos[i] = rand() % (length - 1) + 1;
			for (int j = i - 1; j >= 0; j--) {
				while (newpos[j] == newpos[i])
					newpos[i] = rand() % length;
			}//找到互不相同的3个位置
			this->alive[sum + i] = true;//还没被吃掉
			this->score[sum + i] = rand() % 3 + 5;//随机分数
			this->good_bad[sum + i] = rand() % 2;//好坏果子
			//根据好坏属性设置颜色
			if (good_bad[sum + i] == true) {//good
				this->color[sum + i] = good_food_color[rand() % 5];
				this->left++;
			}
			else//bad
				this->color[sum + i] = RGB(139, 137, 112);
			food_pos[sum + i] = snake_pos[newpos[i]];
		}
		this->sum += 3;
	}
}

/*******************************WALL类成员函数的实现*************************************/
WALL::WALL() {
	this->wall_num = 0;
	this->wall_radio = 5;
}
//将原本死掉蛇蛇的位置随机生成软墙壁和硬墙壁
void WALL::wall_build(vector<pos>dead_snake_pos) {
	//随机确定本次生成的墙是软墙壁还是硬墙壁
	bool soft_hard = rand() % 2;
	int num = (int)dead_snake_pos.size();//确定一共新增多少块墙壁
	for (int i = 0; i < num; i++) {
		this->wall_num++;
		this->wall_pos.push_back(dead_snake_pos[i]);
		this->wall_soft_hard.push_back(soft_hard);
		//依据软硬墙壁决定颜色种类
		if (soft_hard) {//soft_hard=true为软墙壁
			wall_color.push_back(RGB(217, 217, 217));
		}
		else {//硬墙壁的颜色比软墙壁颜色更深
			wall_color.push_back(RGB(166, 166, 166));
		}
	}
}

//绘制所有墙壁
void WALL::wall_render() {
	for (int i = 0; i < wall_num; i++) {
		setfillcolor(this->wall_color[i]);
		int x = this->wall_pos[i].x - wall_radio;
		int y = this->wall_pos[i].y - wall_radio;
		solidroundrect(x, y, x + 2 * wall_radio, y + 2 * wall_radio, 3, 3);
	}
}
//判断当前墙壁是软墙壁还是硬墙壁
bool WALL::wall_getQuality(int wall_index) {
	return this->wall_soft_hard[wall_index];
}
//检查新生成的食物是否出现在了墙壁上
bool WALL::wall_checkpos(int food_x, int food_y) {
	for (int i = 0; i < (int)this->wall_pos.size(); i++) {
		if (food_x == this->wall_pos[i].x && food_y == wall_pos[i].y)
			return false;
	}
	return true;
}

//重置墙壁属性
void WALL::wall_reset() {
	this->wall_num = 0;
	this->wall_soft_hard.clear();
	this->wall_color.clear();
	this->wall_pos.clear();
}