#include"manager.h"
#include"Record.h"

//manager类构造函数
MANAGER::MANAGER() {
	//snake的构造函数已经隐式调用完毕
	//wall的构造函数已经隐式调用完毕
	//food调用的是无参空体，所以需要显示设置
	food.radio = 6;
	food.food_set(snake, wall);
	//初始将胜者定为0，最终player1赢则winner=1 player2赢则winner=2
	winner = 0;
	//初始认为不查看排行榜
	checkRank = false;
	//加载原有的排行榜
	this->manager_readranking();
}
//单机游戏时进入自动游戏模式：单机ai模式或人机对战模式
void MANAGER::manager_ai() {
	//找到最近的好果子作为target
	int target = 0;
	int index = 0;
	//蛇头的坐标
	int head_x = snake.snake_pos[snake.mode][0].x;
	int head_y = snake.snake_pos[snake.mode][0].y;
	int min_d = INT_MAX;
	if (food.left > 0) {
		while (true) {
			if (index >= food.sum)
				break;
			if (food.food_getQuality(index))
			{
				if (food.food_getAlive(index)) {
					int food_x = food.food_pos[index].x;
					int food_y = food.food_pos[index].y;
					int distance = abs(head_x - food_x) + abs(head_y - food_y);
					if (distance < min_d)
						target = index;
				}
			}
			index++;
		}
		if (target < food.sum)
			snake.snake_easyAI(snake.mode, food.food_pos[target]);
	}
}

//渲染过程
void MANAGER::manager_render() {
	if (this->snake.mode == MODE::multi)
		Sleep(10);
	this->wall.wall_render();
	this->food.food_render();
	this->manager_draw_info();
	for (int i = 0; i <= (int)snake.mode; i++) {
		this->snake.snake_bodyRender(i);
	}
}
//处理事件
void MANAGER::manager_event(BYTE& vkcode) {
	if (snake.mode == MODE::multi && snake.cgtoAI == true)//人机对战模式
		this->snake.snake_event(0, vkcode);
	else {//单机自主模式或双人模式
		for (int i = 0; i <= snake.mode; i++)
			this->snake.snake_event(i, vkcode);
	}
}
//处理吃食物
void MANAGER::manager_eatfood() {
	for (int i = 0; i <= snake.mode; i++) {
		//蛇的坐标和食物的坐标相等就是发生了吃食物事件
		pos snakepos = this->snake.snake_pos[i][0];//某条蛇的蛇头坐标
		//当前页面中的每一个食物都要遍历判断一次
		for (int foodnum = 0; foodnum < food.sum; foodnum++) {
			pos foodpos = this->food.food_pos[foodnum];//某一个食物的坐标
			int r = food.radio * 2;//获取食物半径
			if (food.left == 0)//所有好果子都已经被吃掉
			{
				food.food_set(snake, wall);//重置食物状态
				break;
			}
			if (snakepos.x >= foodpos.x - r && snakepos.x <= foodpos.x + r && snakepos.y >= foodpos.y - r && snakepos.y <= foodpos.y + r
				&& food.food_getAlive(foodnum)) {
				//相撞状态,吃到食物
				food.food_setAlive(foodnum, false);//食物的alive属性改变

				food.color[foodnum] = RGB(245, 245, 245);//食物的颜色设为背景色(好像也不用设置。。。)

				//依据当前这个食物的属性作蛇的加分减分\长度增减等处理
				int change = food.food_getScore(foodnum) - 4;
				if (food.food_getQuality(foodnum)) {//好果子
					snake.snake_increase(i, change);
					food.left--;//剩余食物数量减1
				}
				else {//坏果子
					snake.snake_decrease(i, change);
				}

			}
		}
	}
}
//事件更新
void MANAGER::manager_update() {
	if (snake.cgtoAI) {
		manager_ai();
	}
	for (int i = 0; i <= snake.mode; i++)
		this->snake.snake_move(i);
}
//重置时间
void MANAGER::manager_reset_time() {
	this->time = clock();//游戏开始使获取当前时间
}
//游戏进行中的相关信息展示
void MANAGER::manager_draw_info() {

	//从游戏开始到当前时刻已经经过的时间
	int passtime = (clock() - this->time) / CLOCKS_PER_SEC;
	int minute = passtime / 60;//已经经过的分钟数
	int second = passtime % 60;//已经经过的秒数
	TCHAR myTime[15] = { 0 };
	_stprintf(myTime, _T("%2dmin%02ds"), minute, second);

	if (snake.mode == MODE::easy) {
		//显示单机模式下的时间显示
		settextcolor(RGB(143, 115, 66));
		settextstyle(40, 0, _T("Arial BLACK"));
		setbkmode(TRANSPARENT);
		outtextxy(760, 170, myTime);

		//显示当前得分
		TCHAR myScore[5] = { 0 };
		_stprintf(myScore, _T("%d"), this->snake.score[0]);
		outtextxy(600, 240, myScore);

		//打印当前模式下的历史最高分
		TCHAR maxscore[5] = { 0 };
		easy_max = max(easy_max, snake.score[0]);//若当前得分一直在刷新历史记录，历史最高分同步刷新
		_stprintf(maxscore, _T("%d"), easy_max);
		outtextxy(760, 240, maxscore);

		//能量槽底色
		setfillcolor(WHITE);
		solidroundrect(600, 110, 800, 130, 3, 3);

		//显示剩余生命数和能量槽
		TCHAR myLife[5] = { 0 };
		_stprintf(myLife, _T("%d"), this->snake.life[0]);
		//依据生命数的多少设置显示颜色
		if (this->snake.life[0] <= 10) {
			setfillcolor(RGB(205, 51, 51));
			settextcolor(RGB(205, 51, 51));//生命过少时用红色显示
		}
		else if (this->snake.life[0] > 10 && this->snake.life[0] <= 50) {
			setfillcolor(RGB(255, 130, 71));
			settextcolor(RGB(255, 130, 71));
		}//较为安全时用黄色显示
		else {
			setfillcolor(RGB(154, 205, 50));
			settextcolor(RGB(154, 205, 50));
		}//完全安全时用绿色显示

		outtextxy(600, 173, myLife);//打印生命数
		if (snake.score[0] <= 660)
			solidroundrect(600, 110, 600 + snake.length[0] * 3, 130, 3, 3);//打印能量槽
		else
			solidroundrect(600, 110, 800, 130, 3, 3);

		//打印小蛇的长度
		settextcolor(RGB(143, 115, 66));
		TCHAR myLen[5] = { 0 };
		_stprintf(myLen, _T("%d"), this->snake.length[0]);
		outtextxy(810, 100, myLen);
	}
	else if (snake.mode == MODE::multi) {
		//显示双蛇模式下的时间显示
		settextcolor(RGB(143, 115, 66));
		settextstyle(30, 0, _T("Arial BLACK"));
		setbkmode(TRANSPARENT);
		outtextxy(47, 88, myTime);

		//能量槽底色
		setfillcolor(WHITE);
		solidroundrect(540, 85, 740, 105, 3, 3);//蓝色小蛇的能量槽
		solidroundrect(220, 85, 420, 105, 3, 3);//红色小蛇的能量槽
		//打印能量槽
		for (int i = 0; i < 2; i++) {
			//依据生命数的多少设置显示颜色
			if (this->snake.life[i] <= 10)
				setfillcolor(RGB(205, 51, 51));
			else if (this->snake.life[i] > 10 && this->snake.life[i] <= 50) //较为安全时用黄色显示
				setfillcolor(RGB(255, 130, 71));
			else//完全安全时用绿色显示
				setfillcolor(RGB(154, 205, 50));
			if (i == 1) {
				if (snake.score[1] <= 660)
					solidroundrect(540, 85, 540 + snake.length[1] * 3, 105, 3, 3);
				else
					solidroundrect(540, 85, 740, 105, 3, 3);
			}//打印蓝色小蛇能量槽
			else {
				if (snake.score[0] <= 660)
					solidroundrect(420 - snake.length[0] * 3, 85, 420, 105, 3, 3);
				else
					solidroundrect(220, 85, 420, 105, 3, 3);
			}//打印红色小蛇能量槽
		}
		//打印分数比
		settextstyle(30, 0, _T("Arial BLACK"));
		outtextxy(475, 80, _T(":"));
		TCHAR score1[5] = { 0 };
		TCHAR score2[5] = { 0 };
		_stprintf(score1, _T("%d"), this->snake.score[0]);
		_stprintf(score2, _T("%d"), this->snake.score[1]);
		outtextxy(430, 80, score1);
		outtextxy(485, 80, score2);
	}
}
//处理撞墙
void MANAGER::manager_crashwall() {
	int prespeed[2];
	prespeed[0] = snake.speed[0];
	prespeed[1] = snake.speed[1];
	bool alive = true;
	for (int i = 0; i <= snake.mode; i++) {
		alive = true;
		//蛇头坐标
		pos snakepos = this->snake.snake_pos[i][0];

		//当前页面中的所有墙壁都要遍历判断一次
		for (size_t wallnum = 0; wallnum < this->wall.wall_pos.size(); wallnum++) {
			pos wallpos = this->wall.wall_pos[wallnum];//某一块墙壁的坐标
			int r = snake.radio;//墙壁的厚度等于蛇的半径
			if (snakepos.x >= wallpos.x - r && snakepos.x <= wallpos.x + r
				&& snakepos.y >= wallpos.y - r && snakepos.y <= wallpos.y + r) {
				//发生了撞墙事件

				if (this->wall.wall_getQuality(wallnum)) {
					int prelength = this->snake.length[i];
					//撞到了软墙壁
					HWND h = GetHWnd();
					MessageBox(h, _T("oops!!!蛇蛇撞到了【软墙壁】！\n你将回到起点继续本轮游戏~\n>>按下回车[确定]继续游戏"), _T("GAME-蛇蛇撞墙提示"), MB_OK);
					this->snake.snake_reset(i);
					if (prelength > 5) {
						snake.snake_increase(i, prelength - 5);
					}
					else if (prelength == 4) {
						snake.snake_decrease(i, prelength - 3);
					}
					else if (prelength == 3) {
						snake.snake_decrease(i, 2);
					}
					break;
				}
				else {
					//撞到硬墙壁小蛇直接死
					HWND h = GetHWnd();
					MessageBox(h, _T("oops!!!蛇蛇撞到了【硬墙壁】！\n>>很遗憾本轮游戏结束啦~"), _T("GAME-蛇蛇撞墙提示"), MB_OK);
					this->snake.life[i] = 0;//生命值降为0
					alive = false;
					this->manager_multi_setwinner(i + 1);//设置胜者
					break;
				}
			}
			else
				snake.speed[i] = prespeed[i];
		}
		if (alive = false)
			break;
	}
	return;
}
//判断当前这局游戏是否还要继续
bool MANAGER::manager_isRunning() {
	switch (this->snake.mode) {
	case MODE::easy:
		return snake.snake_checklife(0);
		break;
	case MODE::multi:
		return snake.snake_checklife(0) && snake.snake_checklife(1);
		break;
	default:
		return false;
	}
}
//双人模式下设定胜者
void MANAGER::manager_multi_setwinner(int index) {
	if (index == 1)
		this->winner = 2;
	else if (index == 2)
		this->winner = 1;
	else
		this->winner = 0;
}
//展示排行榜
void MANAGER::manager_readranking() {
	//以二进制读入方式打开文件
	ifstream ranking;
	ranking.open("rank.txt", ios::binary);
	if (ranking.is_open()) {//确保文件正确打开
		//简单模式
		ranking >> easy_num;
		for (int i = 0; i < easy_num; i++) {
			ranking >> easy_rank[i];//读入用户名

			int inlevel = 0;
			ranking >> inlevel;
			level[0][i] = (Level)inlevel;//读入游戏难度

			ranking >> score[0][i];//读入分数
		}
		//人机模式
		ranking >> ai_num;
		for (int i = 0; i < ai_num; i++) {
			ranking >> ai_rank[i];//读入用户名

			int inlevel = 0;
			ranking >> inlevel;
			level[1][i] = (Level)inlevel;//读入游戏难度

			ranking >> score[1][i];//读入分数
		}
		//双人模式
		ranking >> multi_num;
		for (int i = 0; i < multi_num; i++) {
			ranking >> multi_rank[i];//读入用户名

			int inlevel = 0;
			ranking >> inlevel;
			level[2][i] = (Level)inlevel;//读入游戏难度

			ranking >> score[2][i];//读入分数

		}
		ranking.close();//关闭文件
	}
}
void judge_edition(Level inlevel, TCHAR myEdition[]) {
	switch (inlevel) {
	case Level::first:
		wcscpy(myEdition, _T("[Level 1]"));
		break;
	case Level::second:
		wcscpy(myEdition, _T("[Level 2]"));
		break;
	case Level::third:
		wcscpy(myEdition, _T("[Level 3]"));
		break;
	}
}

//打印排行榜
void MANAGER::manager_renderRanking() {

	//读取文件
	this->manager_readranking();

	//输出到屏幕端的用户名
	TCHAR show_user[15] = { 0 };
	//输出到屏幕端的版本
	TCHAR show_edition[15] = { 0 };
	//输出到屏幕端的分数
	TCHAR show_score[5] = { 0 };

	//输出简单模式的排行榜
	for (int i = 0; i < easy_num; i++) {
		//字节向宽字节转换
		//char类型的用户名转换为宽字节
		MultiByteToWideChar(CP_ACP, 0, easy_rank[i], -1, show_user, 15);
		//难度版本
		judge_edition(level[0][i], show_edition);

		//分数
		_stprintf(show_score, _T("%d"), score[0][i]);

		//输出
		settextcolor(RGB(143, 115, 66));
		settextstyle(20, 0, _T("Arial BLACK"));
		setbkmode(TRANSPARENT);
		outtextxy(100, 235 + i * 108, show_user);
		outtextxy(100, 270 + i * 108, show_edition);
		outtextxy(180, 270 + i * 108, show_score);
	}
	//输出人机模式的排行榜
	for (int i = 0; i < ai_num; i++) {
		//字节向宽字节转换
		//char类型的用户名转换为宽字节
		MultiByteToWideChar(CP_ACP, 0, ai_rank[i], -1, show_user, 15);
		//难度版本
		judge_edition(level[1][i], show_edition);
		//分数
		_stprintf(show_score, _T("%d"), score[1][i]);

		//输出
		settextcolor(RGB(143, 115, 66));
		settextstyle(20, 0, _T("Arial BLACK"));
		setbkmode(TRANSPARENT);
		outtextxy(410, 235 + i * 108, show_user);
		outtextxy(410, 270 + i * 108, show_edition);
		outtextxy(490, 270 + i * 108, show_score);
	}
	//输出双人模式的排行榜
	for (int i = 0; i < multi_num; i++) {
		//字节向宽字节转换
		//char类型的用户名转换为宽字节
		MultiByteToWideChar(CP_ACP, 0, multi_rank[i], -1, show_user, 15);
		//难度版本
		judge_edition(level[2][i], show_edition);
		//分数
		_stprintf(show_score, _T("%d"), score[2][i]);

		//输出
		settextcolor(RGB(143, 115, 66));
		settextstyle(20, 0, _T("Arial BLACK"));
		setbkmode(TRANSPARENT);
		outtextxy(720, 235 + i * 108, show_user);
		outtextxy(720, 270 + i * 108, show_edition);
		outtextxy(800, 270 + i * 108, show_score);
	}
}