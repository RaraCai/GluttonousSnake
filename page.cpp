#include"myGUI.h"
#include"manager.h"

extern MANAGER manager;

wstring name[4] = { _T("bk_easy_red.png"),_T("bk_easy_yellow.png"),_T("bk_easy_blue.png"),_T("bk_multi.png") };
PAGE menu(960, 540, menu_page, _T("bk_paper.png"));
PAGE game_easy(960, 540, easy_game_page, name[0]);
PAGE game_multi(960, 540, multi_game_page, name[3]);
PAGE game_ai(960, 540, multi_game_page, _T("bk_ai.png"));
PAGE ranking(960, 540, rank_page, _T("bk_rank.png"));

//菜单界面
void myMenu() {

	bool ai = manager.snake.cgtoAI;
	bool notset = (manager.snake.mode == MODE::multi) && ai;

	//回到菜单重置双人模式的Ai属性
	manager.snake.cgtoAI = false;
	//对当前地图作清空处理
	manager.food.food_set(manager.snake, manager.wall);
	manager.wall.wall_reset();
	for (int i = 0; i < manager.snake.mode; i++)
		manager.snake.snake_reset(i);

	//弹窗提示
	HWND h = GetHWnd();
	if (MessageBox(h, _T("你即将返回主页！\n>>[确定]-返回主页 [取消]-保留在当前页面"), _T("GAME-游戏提示"), MB_OKCANCEL) == IDOK) {
		//查看排行榜的标记量设为false
		manager.checkRank = false;
		menu.draw();
		menu.run();
	}
	else {
		if (notset)
			manager.snake.cgtoAI = ai;
	}
}
//一条蛇的界面
void myGame_easy() {
	manager.snake.mode = easy;
	manager.snake.crashtimes[0] = manager.snake.crashtimes[1] = 0;
	wchar_t level[2] = { 0 };
	InputBox(level, 2, _T("欢迎来到蛇蛇的世界！\n[GUIDE]请输入游戏难度：\n  [1]-入门版\n  [2]-进阶版\n  [3]-高级版"), _T("简单模式-难度选择"));
	//输入单机模式下的用户名
	wchar_t user[15] = { 0 };
	InputBox(user, 15, _T("在蛇蛇的世界留下你的名字吧！\n>>请输入本轮游戏你要使用的用户名:\n>>蛇蛇只认识【英文】，请不要超过15个字符~"), _T("简单模式-用户名输入"));
	wcscpy(manager.user1, user);
	//设置游戏难度
	if (level[0] == '1')
		manager.snake.level = Level::first;
	else if (level[0] == '2')
		manager.snake.level = Level::second;
	else if (level[0] == '3')
		manager.snake.level = Level::third;
	//绘制界面
	game_multi.draw();
	//重置游戏时间
	manager.manager_reset_time();

	game_easy.run();

}
//双人对战界面
void myGame_multi() {
	//基本信息设置
	manager.snake.mode = multi;
	manager.snake.crashtimes[0] = manager.snake.crashtimes[1] = 0;

	//输入双人模式下的用户名
	wchar_t user1[16] = { 0 }, user2[16] = { 0 };
	InputBox(user1, 16, _T("在蛇蛇的世界留下你的名字吧！\n>>请输入本轮游戏【Player1】的用户名:\n>>蛇蛇只认识【英文】，请不要超过15个字符~"), _T("双人模式-用户名输入"));
	wcscpy(manager.user1, user1);
	InputBox(user2, 16, _T("在蛇蛇的世界留下你的名字吧！\n>>请输入本轮游戏【Player2】的用户名:\n>>蛇蛇只认识【英文】，请不要超过15个字符~"), _T("双人模式-用户名输入"));
	wcscpy(manager.user2, user2);

	//设置游戏难度
	wchar_t level[2] = { 0 };
	InputBox(level, 2, _T("BATTLE升级！你们要挑战哪种难度？\n[GUIDE]请输入游戏难度：\n  [1]-入门版\n  [2]-进阶版\n  [3]-高级版"), _T("双人模式-难度选择"));
	if (level[0] == '1')
		manager.snake.level = Level::first;
	else if (level[0] == '2')
		manager.snake.level = Level::second;
	else if (level[0] == '3')
		manager.snake.level = Level::third;

	//绘制界面
	game_multi.draw();

	//重置游戏时间
	manager.manager_reset_time();
	game_multi.run();
}
//人机对战界面
void myGame_ai() {
	//基本信息设置
	manager.snake.mode = multi;
	manager.snake.crashtimes[0] = manager.snake.crashtimes[1] = 0;
	//设置AI控制信息
	manager.snake.cgtoAI = true;
	//输入人机对战模式下的用户名
	wchar_t user1[15] = { 0 };
	InputBox(user1, 15, _T("在蛇蛇的世界留下你的名字吧！\n>>请输入本轮游戏【Player】的用户名:\n>>蛇蛇只认识【英文】，请不要超过15个字符~"), _T("人机模式-用户名输入"));
	wcscpy(manager.user1, user1);
	wcscpy(manager.user2, _T("AI-Player"));

	//设置游戏难度
	wchar_t level[2] = { 0 };
	InputBox(level, 2, _T("BATTLE升级！想要挑战哪种难度？\n[GUIDE]请输入游戏难度：\n  [1]-入门版\n  [2]-进阶版\n  [3]-高级版"), _T("人机模式-难度选择"));
	if (level[0] == '1')
		manager.snake.level = Level::first;
	else if (level[0] == '2')
		manager.snake.level = Level::second;
	else if (level[0] == '3')
		manager.snake.level = Level::third;

	//绘制界面
	game_ai.draw();

	//重置游戏时间
	manager.manager_reset_time();
	game_ai.run();
}
//帮助界面
void myHelp() {
	//帮助界面的弹窗
	HWND h = GetHWnd();
	MessageBox(h, _T("【单机模式】:\n>>刚入蛇门？从简单模式开始吧！[1]入门版 [2]进阶版 [3]高级版\n【人机模式】:\n>>想要挑战计算机的算力？来人机模式和AI_snake一起battle吧~\n【双人模式】:\n>>想和室友组团开吃？来试试双人模式！\n\n>>你可以在【排行榜】中查看你的过往历史成绩哦！"), _T("HELP详情"), MB_OK);
}
//退出菜单界面
void quitMenu() {
	closegraph();
	exit(0);
}
//规则界面
void myRule() {
	//规则界面的弹窗
	HWND h = GetHWnd();
	if (manager.snake.level == Level::first)
		MessageBox(h, _T("【入门版】游戏规则：\n>>使用【方向键】控制小蛇移动\n>>按下【空格】小蛇加速，再次按下【空格】恢复原速\n>>【彩色】果实加分，【灰色】果实失分\n>>每次只有将所有【彩色】果实吃光，才会产生新果实哦！\n>>小心控制蛇蛇，一旦【撞墙】游戏将结束！\n>>当你的【能量槽】或【分数】为0时，游戏也将结束！"), _T("GAME-游戏规则"), MB_OK);
	else if (manager.snake.level == Level::second)
		MessageBox(h, _T("【进阶版】游戏规则：\n>>使用【方向键】控制小蛇移动\n>>按下【空格】小蛇加速，再次按下【空格】恢复原速\n>>【彩色】果实加分，【灰色】果实失分\n>>每次只有将所有【彩色】果实吃光，才会产生新果实哦！\n>>小心控制蛇蛇，一旦【撞墙】蛇身将变成【墙壁】，并回到初始位置重新开始！\n>>【浅灰色】为【软墙壁】，会导致你失去一定分数！\n>>【深灰色】为【硬墙壁】，会直接结束游戏！\n>>当你的【能量槽】或【分数】为0时，游戏也将结束！"), _T("GAME-游戏规则"), MB_OK);
	else if (manager.snake.level == Level::third)
		MessageBox(h, _T("【高级版】游戏规则：\n>>使用【方向键】控制小蛇移动\n>>按下【空格】小蛇加速，再次按下【空格】恢复原速\n>>【彩色】果实加分，【灰色】果实失分\n>>每次只有将所有【彩色】果实吃光，才会产生新果实哦！\n>>小心控制蛇蛇，一旦【撞墙】蛇身将变成【果实】，并回到初始位置重新开始！\n>>当撞墙次数超过【5次】，游戏将结束！\n>>当你的【能量槽】或【分数】为0时，游戏也将结束！"), _T("GAME-游戏规则"), MB_OK);
}
//排行榜界面
void myRank() {
	manager.checkRank = true;
	ranking.draw();
	ranking.run();
}