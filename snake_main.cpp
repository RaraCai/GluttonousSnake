#include"myGUI.h"
#include"page.h"

MANAGER manager;


int main() {

	//随机生成函数的种子
	srand((unsigned int)time(NULL));
	//各页面GUI的初始化
	menu.init_page();
	menu.add_Button();
	game_easy.add_Button();
	game_multi.add_Button();
	game_ai.add_Button();
	ranking.add_Button();
	//绘制菜单界面
	menu.draw();
	//主页面的弹窗
	HWND h = GetHWnd();
	MessageBox(h, _T("欢迎来到蛇蛇的世界！\n若你选择【单机模式】:\n>>可以选择你喜欢的小蛇【皮肤】！\n若你选择【人机模式】或【双人模式】:\n>>程序媛姐姐还没有为你开设自选皮肤的权限~\n[HELP]:更多帮助请在【HELP】键查看~"), _T("菜单页-NOTICE"), MB_OK);
	//进入菜单界面开始运行
	menu.run();
}