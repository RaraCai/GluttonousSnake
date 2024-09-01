#define _CRT_SECURE_NO_WARNINGS
//�����¼
#include<fstream>
#include"item.h"

//��¼������Ϸ��¼���ļ���
fstream record("record.txt", ios::binary | ios::app);
//4��ģʽ�µ���ʷ��߷�
int easy_max = 0;
int ai_max = 0;
int multi_max = 0;
//д����ʷ��¼���ļ���
void write_record(PageType pagetype, MODE mode, Level level, char user1[], int score1, int score2 = 0, char user2[] = (char*)"AI_snake") {
	if (mode == MODE::easy) {//����ģʽ
		//����û���
		record << "[�û���]:" << user1 << " [�汾]:";
		if (pagetype == PageType::easy_game_page) {//��ģʽ
			switch (level) {
			case Level::first:
				record << "����ģʽ-���Ű� ";
				break;
			case Level::second:
				record << "����ģʽ-���װ� ";
				break;
			case Level::third:
				record << "����ģʽ-�߼��� ";
				break;
			}
			//������ʷ��¼
			easy_max = max(easy_max, score1);
		}
		record << "[�÷�]:" << score1 << endl;
	}
	else if (mode == MODE::multi) {//˫��ģʽ��AIģʽ
		if (user2 != "AI_snake") {//˫��ģʽ
			record << "[�û���1]:" << user1 << " [�汾]:˫��ģʽ [�÷�]:" << score1 << endl;
			record << "[�û���2]:" << user2 << " [�汾]:˫��ģʽ [�÷�]:" << score2 << endl;
			//������ʷ��¼
			score1 = max(score1, score2);
			multi_max = max(score1, multi_max);
		}
		else {//AI�˻���սģʽ
			record << "[�û���1]:" << user1 << " [�汾]:AIģʽ-�˻���ս [�÷�]:" << score1 << endl;
			record << "[�û���2]:AI_snake [�汾]:AIģʽ-�˻���ս [�÷�]:" << score2 << endl;
			//������ʷ��¼
			ai_max = max(ai_max, score1);
		}
	}
}


//��¼���а���ļ���

char easy_rank[3][15] = { 0 };//��ģʽ�µ�ǰ3��
char ai_rank[3][15] = { 0 };//aiģʽ�µ�ǰ3��
char multi_rank[3][15] = { 0 };//˫��ģʽ�µ�ǰ����
int easy_num = 0, ai_num = 0, multi_num = 0;//��¼��ǰ�ڰ��ϵĸ���ģʽ�µ���Ҹ��������ڶ�ȡ�ļ�ʱ�Ĳ�������
int score[3][3] = { 0 };//3��ģʽ�¶�Ӧǰ�����ķ���
Level level[3][3];
//������ģʽ�µ�ǰ����д��rank.txt
/*ÿ��ģʽ�£����а��ļ�������
* num
* username1 level1 score1
* username2 level2 score2
* username3 level3 score3�ĸ�ʽ�洢
* AI�˻���սģʽ����дlevel
*/
//������
void sort_rank(char name[][15], int score[], Level level[]) {
	for (int i = 0; i < 3; i++) {
		for (int j = i + 1; j < 3; j++) {
			if (score[j] > score[i]) {
				swap(score[i], score[j]);//��������
				swap(level[i], level[j]);//������Ϸģʽ

				char tmp[15] = { 0 };
				strcpy(tmp, name[i]);
				strcpy(name[i], name[j]);
				strcpy(name[j], tmp);//�����û���
			}
		}
	}
}
//�ļ�д�뺯��
void write_rank() {

	ofstream ranking("rank.txt", ios::binary);

	if (ranking.is_open()) {//�ж��ļ��Ƿ�ɹ���

		//�ȶԵõ��������������
		sort_rank(easy_rank, score[0], level[0]);//����
		sort_rank(ai_rank, score[1], level[1]);//�˻�
		sort_rank(multi_rank, score[2], level[2]);//˫��

		//��ģʽ
		ranking << ((easy_num > 3) ? 3 : easy_num) << endl;
		for (int i = 0; i < easy_num; i++) {
			//��ӡ�û���
			ranking << easy_rank[i] << ' ';
			ranking << (int)level[0][i] << ' ' << score[0][i] << endl;
		}
		//�˻�ģʽ
		ranking << ((ai_num > 3) ? 3 : ai_num) << endl;
		for (int i = 0; i < ai_num; i++) {
			//��ӡ�û���
			ranking << ai_rank[i] << ' ';
			ranking << (int)level[1][i] << ' ' << score[1][i] << endl;
		}
		//˫��ģʽ
		ranking << ((multi_num > 3) ? 3 : multi_num) << endl;
		for (int i = 0; i < multi_num; i++) {
			//��ӡ�û���
			ranking << multi_rank[i] << ' ';
			ranking << (int)level[2][i] << ' ' << score[2][i] << endl;
		}
		ranking.close();//�ر��ļ�
	}
}
//�����³ɼ���ֵ������
void insert_rank(char name[][15], int score[], Level level[], char newname[], Level newlevel, int newscore) {
	for (int i = 0; i < 3; i++) {
		if (score[i] <= newscore) {//�ҵ���ȷ�Ĳ���λ��
			//��i��ʼ������Ԫ�غ���1��
			for (int j = 2; j > i; j--) {
				score[j] = score[j - 1];
				level[j] = level[j - 1];
				strcpy(name[j], name[j - 1]);
			}
			//�ڵ�i��λ�ò����·���
			score[i] = newscore;
			level[i] = newlevel;
			strcpy(name[i], newname);
			//�����������������
			break;
		}
	}
}

//�ȶ��²����Ľ�����ж��Ƿ�Ҫ����
void update_rank(int newscore, char newname[15], Level newlevel, int choice) {
	//����
	//�����ǽ������е�

	if (choice == 1) {//��ģʽ�ĸ���
		if (newscore >= score[0][2])
			insert_rank(easy_rank, score[0], level[0], newname, newlevel, newscore);
	}
	else if (choice == 2) {//AIģʽ�ĸ���
		if (newscore >= score[1][2])
			insert_rank(ai_rank, score[1], level[1], newname, newlevel, newscore);
	}
	else if (choice == 3) {//˫��ģʽ�ĸ���
		if (newscore >= score[2][2])
			insert_rank(multi_rank, score[2], level[2], newname, newlevel, newscore);
	}
}
