#include "myGUI.h"
#include "page.h"

const int border_line[3] = { 139,115,85 };
const int normal_button[3] = { 205,155,29 };
const int light_button[3] = { 255,236,139 };
#define redsnake 0
#define yellowsnake 1
#define bluesnake 2

void red() {

	game_easy.bkpaper_name = name[0];

	HWND h = GetHWnd();
	if (MessageBox(h, _T("你选择的小蛇皮肤为：RED"), _T("皮肤颜色选择"), MB_OK)) {
		manager.snake.set_skincolor(0, redsnake);
		manager.snake.set_skincolor(1, redsnake);
	}
}
void yellow() {
	game_easy.bkpaper_name = name[1];

	HWND h = GetHWnd();
	if (MessageBox(h, _T("你选择的小蛇皮肤为：YELLOW"), _T("皮肤颜色选择"), MB_OK)) {
		manager.snake.set_skincolor(0, yellowsnake);
		manager.snake.set_skincolor(1, yellowsnake);
	}
}
void blue() {
	game_easy.bkpaper_name = name[2];

	HWND h = GetHWnd();
	if (MessageBox(h, _T("你选择的小蛇皮肤为：BLUE"), _T("皮肤颜色选择"), MB_OK)) {
		manager.snake.set_skincolor(0, bluesnake);
		manager.snake.set_skincolor(1, bluesnake);
	}
}
//暂停处理
void myPause() {
	HWND h = GetHWnd();
	MessageBox(h, _T("你已将游戏暂停，按下[确定]键游戏继续"), _T("游戏暂停"), MB_OK);
}
//简易模式的AI蛇
void myeasyAI() {
	if (manager.snake.cgtoAI == true)
		manager.snake.cgtoAI = false;//离开AI模式
	else {
		manager.snake.cgtoAI = true;
		manager.snake.speed[0] = 1;
	}//设置进入AI模式
}
//BUTTON类：构造函数
BUTTON::BUTTON(int x, int y, int width, int height, const wstring& info, void(*myFun)()) {
	pos.first = x;
	pos.second = y;
	size.first = width;
	size.second = height;
	text = info;
	scale = 1.0f;
	isMouseOver = false;
	onClick = myFun;
}
//BUTTON类：绘制按钮（分为悬停与不悬停）
void BUTTON::myGUI_draw_BUTTON() {
	//设置按钮的基本信息
	int width = int(size.first * scale);//缩放后按钮宽度
	int height = int(size.second * scale);//缩放后按钮高度
	int x = pos.first + (size.first - width) / 2;//缩放后按钮的X坐标
	int y = pos.second + (size.second - height) / 2;//缩放后按钮的y坐标

	//设置边框线条颜色
	setlinecolor(RGB(border_line[0], border_line[1], border_line[2]));
	//根据鼠标是否悬停设置按钮填充色
	if (isMouseOver)
		setfillcolor(RGB(light_button[0], light_button[1], light_button[2]));
	else
		setfillcolor(RGB(normal_button[0], normal_button[1], normal_button[2]));


	setbkmode(TRANSPARENT);//设置文本背景为透明
	fillroundrect(x, y, x + width, y + height, 15, 15);
	settextstyle(25, 0, _T("Arial BLACK"), 0, 0, 30, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0);
	settextcolor(WHITE);

	//将文字绘制在矩形中心
	int tx = pos.first + (size.first - textwidth(text.c_str())) / 2;
	int ty = pos.second + (size.second - textheight(text.c_str())) / 2;
	//打印出文本
	outtextxy(tx, ty, text.c_str());
}
//BUTTON类：检查鼠标悬停于按钮上方
void BUTTON::checkMouseOver(int mouseX, int mouseY) {
	isMouseOver = ((mouseX >= pos.first && mouseX <= pos.first + size.first)
		&& (mouseY >= pos.second && mouseY <= pos.second + size.second));

	if (isMouseOver)//鼠标悬停在上方时尺寸缩小
		scale = 0.9f;
	else//不悬停时保持正常尺寸
		scale = 1.0f;
}
//BUTTON类：检查鼠标在按钮内部发生点击时间
bool BUTTON::checkClick(int mouseX, int mouseY) {
	if ((mouseX >= pos.first && mouseX <= pos.first + size.first)
		&& (mouseY >= pos.second && mouseY <= pos.second + size.second)) {
		isMouseOver = true;
		onClick();//执行对应功能指针所指向的函数
		//BUTTON的各项属性恢复到悬停前
		isMouseOver = false;
		scale = 1.0f;
		return true;
	}
	return false;
}


//PAGE类：初始化页面绘制
void PAGE::init_page() {
	initgraph(this->page_width, this->page_height, SHOWCONSOLE);
}
//PAGE类：添加按钮信息并绘制
void PAGE::add_Button() {
	switch (this->pagetype) {
	case menu_page:
	{
		BUTTON red_snake(665, 365, 50, 30, _T("YES"), red);
		this->buttons.push_back(red_snake);
		button_nums++;

		BUTTON yellow_snake(775, 365, 50, 30, _T("YES"), yellow);
		this->buttons.push_back(yellow_snake);
		button_nums++;

		BUTTON blue_snake(885, 365, 50, 30, _T("YES"), blue);
		this->buttons.push_back(blue_snake);
		button_nums++;

		BUTTON easy_mode(630, 440, 85, 35, _T("单人模式"), myGame_easy);
		this->buttons.push_back(easy_mode);
		button_nums++;

		BUTTON hard_mode(730, 440, 85, 35, _T("人机模式"), myGame_ai);
		this->buttons.push_back(hard_mode);
		button_nums++;

		BUTTON player_mode(830, 440, 85, 35, _T("双人模式"), myGame_multi);
		this->buttons.push_back(player_mode);
		button_nums++;

		BUTTON ranking(740, 490, 70, 35, _T("排行榜"), myRank);//待修改
		this->buttons.push_back(ranking);
		button_nums++;

		BUTTON helper(830, 490, 85, 35, _T("HELP"), myHelp);
		this->buttons.push_back(helper);
		button_nums++;

		BUTTON quit(850, 20, 85, 35, _T("点此退出"), quitMenu);
		this->buttons.push_back(quit);
		button_nums++;
		break;
	}
	case easy_game_page:
	{
		BUTTON rule(580, 348, 85, 30, _T("点击查看"), myRule);
		this->buttons.push_back(rule);
		button_nums++;

		BUTTON pause(580, 422, 85, 30, _T("点击暂停"), myPause);
		this->buttons.push_back(pause);
		button_nums++;

		BUTTON ai(580, 493, 85, 30, _T("点击切换"), myeasyAI);//进入AI模式
		this->buttons.push_back(ai);
		button_nums++;

		BUTTON back(20, 15, 95, 30, _T("<返回主页"), myMenu);
		this->buttons.push_back(back);
		button_nums++;
		break;
	}
	case multi_game_page: {
		BUTTON pause(130, 15, 85, 30, _T("点击暂停"), myPause);
		this->buttons.push_back(pause);
		button_nums++;

		BUTTON back(20, 15, 95, 30, _T("<返回主页"), myMenu);
		this->buttons.push_back(back);
		button_nums++;
		break;
	}
	case rank_page: {
		BUTTON back(20, 15, 95, 30, _T("<返回主页"), myMenu);
		this->buttons.push_back(back);
		button_nums++;
		break;
	}
	}
}
void PAGE::draw() {
	//加载背景图
	loadimage(&background, bkpaper_name.c_str(), this->page_width, this->page_height);
	putimage(0, 0, &background);
	//绘制按钮
	for (int index = 0; index < button_nums; index++)
		buttons[index].myGUI_draw_BUTTON();
}
//处理鼠标单击事件
void PAGE::mouseClick(int mouseX, int mouseY) {
	for (int index = 0; index < button_nums; index++) {
		if (buttons[index].checkClick(mouseX, mouseY))
			break;
	}
}
//处理鼠标移动事件
void PAGE::mouseMove(int mouseX, int mouseY) {
	for (int index = 0; index < button_nums; index++) {
		buttons[index].checkMouseOver(mouseX, mouseY);
	}
}

void PAGE::run() {
	BeginBatchDraw();//开始批量绘制
	while (true) {
		if (peekmessage(&msg, EM_KEY | EM_MOUSE)) {
			switch (msg.message) {
			case WM_KEYDOWN://键盘按下事件
				if (manager.snake.cgtoAI == false && manager.snake.mode == MODE::easy || manager.snake.mode == MODE::multi)
					manager.manager_event(msg.vkcode);
				break;
			case WM_LBUTTONDOWN://鼠标左键按下事件
				mouseClick(msg.x, msg.y);//处理鼠标单击
				break;
			case WM_MOUSEMOVE://鼠标移动事件
				mouseMove(msg.x, msg.y);//处理鼠标移动事件
				break;
			}
		}
		draw();
		if (this->pagetype == PageType::easy_game_page || this->pagetype == PageType::multi_game_page) {
			if (manager.manager_isRunning()) {
				manager.manager_crashwall();
				manager.manager_eatfood();
				manager.manager_update();
				manager.manager_render();
			}
			else {
				manager.snake.way_to_exit();
			}
		}
		else if (this->pagetype == PageType::rank_page) {
			manager.manager_renderRanking();
		}
		FlushBatchDraw();//将缓冲区内容显示在屏幕上
	}
	EndBatchDraw();//结束批量绘制
}

