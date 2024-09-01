#pragma once
#include"manager.h"

//弹窗提示和各页面运行函数的声明
void myMenu();
void myGame_easy();
void myGame_multi();
void myGame_ai();
void myHelp();
void myRule();
void myRank();
void quitMenu();

extern PAGE menu;
extern PAGE game_easy;
extern PAGE game_multi;
extern PAGE game_ai;
extern PAGE ranking;

extern MANAGER manager;
extern wstring name[4];
