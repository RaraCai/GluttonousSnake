#pragma once
#include<fstream>
enum MODE;
enum Level;
extern char easy_rank[3][15];
extern char ai_rank[3][15];
extern char multi_rank[3][15];
extern int easy_num, ai_num, multi_num;
extern int score[3][3];
extern Level level[3][3];


void update_rank(int newscore, char newname[15], Level newlevel, int choice);
void write_rank();

extern int easy_max;
extern int ai_max;
extern int multi_max;