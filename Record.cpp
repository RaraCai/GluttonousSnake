#define _CRT_SECURE_NO_WARNINGS
//保存记录
#include<fstream>
#include"item.h"

//记录所有游戏记录的文件流
fstream record("record.txt", ios::binary | ios::app);
//4个模式下的历史最高分
int easy_max = 0;
int ai_max = 0;
int multi_max = 0;
//写入历史记录到文件中
void write_record(PageType pagetype, MODE mode, Level level, char user1[], int score1, int score2 = 0, char user2[] = (char*)"AI_snake") {
	if (mode == MODE::easy) {//单机模式
		//输出用户名
		record << "[用户名]:" << user1 << " [版本]:";
		if (pagetype == PageType::easy_game_page) {//简单模式
			switch (level) {
			case Level::first:
				record << "单人模式-入门版 ";
				break;
			case Level::second:
				record << "单人模式-进阶版 ";
				break;
			case Level::third:
				record << "单人模式-高级版 ";
				break;
			}
			//更新历史记录
			easy_max = max(easy_max, score1);
		}
		record << "[得分]:" << score1 << endl;
	}
	else if (mode == MODE::multi) {//双人模式或AI模式
		if (user2 != "AI_snake") {//双人模式
			record << "[用户名1]:" << user1 << " [版本]:双人模式 [得分]:" << score1 << endl;
			record << "[用户名2]:" << user2 << " [版本]:双人模式 [得分]:" << score2 << endl;
			//更新历史记录
			score1 = max(score1, score2);
			multi_max = max(score1, multi_max);
		}
		else {//AI人机对战模式
			record << "[用户名1]:" << user1 << " [版本]:AI模式-人机对战 [得分]:" << score1 << endl;
			record << "[用户名2]:AI_snake [版本]:AI模式-人机对战 [得分]:" << score2 << endl;
			//更新历史记录
			ai_max = max(ai_max, score1);
		}
	}
}


//记录排行榜的文件流

char easy_rank[3][15] = { 0 };//简单模式下的前3名
char ai_rank[3][15] = { 0 };//ai模式下的前3名
char multi_rank[3][15] = { 0 };//双人模式下的前三名
int easy_num = 0, ai_num = 0, multi_num = 0;//记录当前在榜上的各个模式下的玩家个数，便于读取文件时的操作控制
int score[3][3] = { 0 };//3种模式下对应前三名的分数
Level level[3][3];
//将各种模式下的前三名写入rank.txt
/*每种模式下，排行榜文件都按照
* num
* username1 level1 score1
* username2 level2 score2
* username3 level3 score3的格式存储
* AI人机对战模式不用写level
*/
//排序函数
void sort_rank(char name[][15], int score[], Level level[]) {
	for (int i = 0; i < 3; i++) {
		for (int j = i + 1; j < 3; j++) {
			if (score[j] > score[i]) {
				swap(score[i], score[j]);//交换分数
				swap(level[i], level[j]);//交换游戏模式

				char tmp[15] = { 0 };
				strcpy(tmp, name[i]);
				strcpy(name[i], name[j]);
				strcpy(name[j], tmp);//交换用户名
			}
		}
	}
}
//文件写入函数
void write_rank() {

	ofstream ranking("rank.txt", ios::binary);

	if (ranking.is_open()) {//判断文件是否成功打开

		//先对得到的数组进行排序
		sort_rank(easy_rank, score[0], level[0]);//单机
		sort_rank(ai_rank, score[1], level[1]);//人机
		sort_rank(multi_rank, score[2], level[2]);//双人

		//简单模式
		ranking << ((easy_num > 3) ? 3 : easy_num) << endl;
		for (int i = 0; i < easy_num; i++) {
			//打印用户名
			ranking << easy_rank[i] << ' ';
			ranking << (int)level[0][i] << ' ' << score[0][i] << endl;
		}
		//人机模式
		ranking << ((ai_num > 3) ? 3 : ai_num) << endl;
		for (int i = 0; i < ai_num; i++) {
			//打印用户名
			ranking << ai_rank[i] << ' ';
			ranking << (int)level[1][i] << ' ' << score[1][i] << endl;
		}
		//双人模式
		ranking << ((multi_num > 3) ? 3 : multi_num) << endl;
		for (int i = 0; i < multi_num; i++) {
			//打印用户名
			ranking << multi_rank[i] << ' ';
			ranking << (int)level[2][i] << ' ' << score[2][i] << endl;
		}
		ranking.close();//关闭文件
	}
}
//依据新成绩的值做插入
void insert_rank(char name[][15], int score[], Level level[], char newname[], Level newlevel, int newscore) {
	for (int i = 0; i < 3; i++) {
		if (score[i] <= newscore) {//找到正确的插入位置
			//从i开始的所有元素后移1个
			for (int j = 2; j > i; j--) {
				score[j] = score[j - 1];
				level[j] = level[j - 1];
				strcpy(name[j], name[j - 1]);
			}
			//在第i个位置插入新分数
			score[i] = newscore;
			level[i] = newlevel;
			strcpy(name[i], newname);
			//插入结束后跳出即可
			break;
		}
	}
}

//比对新产生的结果，判断是否要更新
void update_rank(int newscore, char newname[15], Level newlevel, int choice) {
	//更新
	//数组是降序排列的

	if (choice == 1) {//简单模式的更新
		if (newscore >= score[0][2])
			insert_rank(easy_rank, score[0], level[0], newname, newlevel, newscore);
	}
	else if (choice == 2) {//AI模式的更新
		if (newscore >= score[1][2])
			insert_rank(ai_rank, score[1], level[1], newname, newlevel, newscore);
	}
	else if (choice == 3) {//双人模式的更新
		if (newscore >= score[2][2])
			insert_rank(multi_rank, score[2], level[2], newname, newlevel, newscore);
	}
}
