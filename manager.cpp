#include"manager.h"
#include"Record.h"

//manager�๹�캯��
MANAGER::MANAGER() {
	//snake�Ĺ��캯���Ѿ���ʽ�������
	//wall�Ĺ��캯���Ѿ���ʽ�������
	//food���õ����޲ο��壬������Ҫ��ʾ����
	food.radio = 6;
	food.food_set(snake, wall);
	//��ʼ��ʤ�߶�Ϊ0������player1Ӯ��winner=1 player2Ӯ��winner=2
	winner = 0;
	//��ʼ��Ϊ���鿴���а�
	checkRank = false;
	//����ԭ�е����а�
	this->manager_readranking();
}
//������Ϸʱ�����Զ���Ϸģʽ������aiģʽ���˻���սģʽ
void MANAGER::manager_ai() {
	//�ҵ�����ĺù�����Ϊtarget
	int target = 0;
	int index = 0;
	//��ͷ������
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

//��Ⱦ����
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
//�����¼�
void MANAGER::manager_event(BYTE& vkcode) {
	if (snake.mode == MODE::multi && snake.cgtoAI == true)//�˻���սģʽ
		this->snake.snake_event(0, vkcode);
	else {//��������ģʽ��˫��ģʽ
		for (int i = 0; i <= snake.mode; i++)
			this->snake.snake_event(i, vkcode);
	}
}
//�����ʳ��
void MANAGER::manager_eatfood() {
	for (int i = 0; i <= snake.mode; i++) {
		//�ߵ������ʳ���������Ⱦ��Ƿ����˳�ʳ���¼�
		pos snakepos = this->snake.snake_pos[i][0];//ĳ���ߵ���ͷ����
		//��ǰҳ���е�ÿһ��ʳ�ﶼҪ�����ж�һ��
		for (int foodnum = 0; foodnum < food.sum; foodnum++) {
			pos foodpos = this->food.food_pos[foodnum];//ĳһ��ʳ�������
			int r = food.radio * 2;//��ȡʳ��뾶
			if (food.left == 0)//���кù��Ӷ��Ѿ����Ե�
			{
				food.food_set(snake, wall);//����ʳ��״̬
				break;
			}
			if (snakepos.x >= foodpos.x - r && snakepos.x <= foodpos.x + r && snakepos.y >= foodpos.y - r && snakepos.y <= foodpos.y + r
				&& food.food_getAlive(foodnum)) {
				//��ײ״̬,�Ե�ʳ��
				food.food_setAlive(foodnum, false);//ʳ���alive���Ըı�

				food.color[foodnum] = RGB(245, 245, 245);//ʳ�����ɫ��Ϊ����ɫ(����Ҳ�������á�����)

				//���ݵ�ǰ���ʳ����������ߵļӷּ���\���������ȴ���
				int change = food.food_getScore(foodnum) - 4;
				if (food.food_getQuality(foodnum)) {//�ù���
					snake.snake_increase(i, change);
					food.left--;//ʣ��ʳ��������1
				}
				else {//������
					snake.snake_decrease(i, change);
				}

			}
		}
	}
}
//�¼�����
void MANAGER::manager_update() {
	if (snake.cgtoAI) {
		manager_ai();
	}
	for (int i = 0; i <= snake.mode; i++)
		this->snake.snake_move(i);
}
//����ʱ��
void MANAGER::manager_reset_time() {
	this->time = clock();//��Ϸ��ʼʹ��ȡ��ǰʱ��
}
//��Ϸ�����е������Ϣչʾ
void MANAGER::manager_draw_info() {

	//����Ϸ��ʼ����ǰʱ���Ѿ�������ʱ��
	int passtime = (clock() - this->time) / CLOCKS_PER_SEC;
	int minute = passtime / 60;//�Ѿ������ķ�����
	int second = passtime % 60;//�Ѿ�����������
	TCHAR myTime[15] = { 0 };
	_stprintf(myTime, _T("%2dmin%02ds"), minute, second);

	if (snake.mode == MODE::easy) {
		//��ʾ����ģʽ�µ�ʱ����ʾ
		settextcolor(RGB(143, 115, 66));
		settextstyle(40, 0, _T("Arial BLACK"));
		setbkmode(TRANSPARENT);
		outtextxy(760, 170, myTime);

		//��ʾ��ǰ�÷�
		TCHAR myScore[5] = { 0 };
		_stprintf(myScore, _T("%d"), this->snake.score[0]);
		outtextxy(600, 240, myScore);

		//��ӡ��ǰģʽ�µ���ʷ��߷�
		TCHAR maxscore[5] = { 0 };
		easy_max = max(easy_max, snake.score[0]);//����ǰ�÷�һֱ��ˢ����ʷ��¼����ʷ��߷�ͬ��ˢ��
		_stprintf(maxscore, _T("%d"), easy_max);
		outtextxy(760, 240, maxscore);

		//�����۵�ɫ
		setfillcolor(WHITE);
		solidroundrect(600, 110, 800, 130, 3, 3);

		//��ʾʣ����������������
		TCHAR myLife[5] = { 0 };
		_stprintf(myLife, _T("%d"), this->snake.life[0]);
		//�����������Ķ���������ʾ��ɫ
		if (this->snake.life[0] <= 10) {
			setfillcolor(RGB(205, 51, 51));
			settextcolor(RGB(205, 51, 51));//��������ʱ�ú�ɫ��ʾ
		}
		else if (this->snake.life[0] > 10 && this->snake.life[0] <= 50) {
			setfillcolor(RGB(255, 130, 71));
			settextcolor(RGB(255, 130, 71));
		}//��Ϊ��ȫʱ�û�ɫ��ʾ
		else {
			setfillcolor(RGB(154, 205, 50));
			settextcolor(RGB(154, 205, 50));
		}//��ȫ��ȫʱ����ɫ��ʾ

		outtextxy(600, 173, myLife);//��ӡ������
		if (snake.score[0] <= 660)
			solidroundrect(600, 110, 600 + snake.length[0] * 3, 130, 3, 3);//��ӡ������
		else
			solidroundrect(600, 110, 800, 130, 3, 3);

		//��ӡС�ߵĳ���
		settextcolor(RGB(143, 115, 66));
		TCHAR myLen[5] = { 0 };
		_stprintf(myLen, _T("%d"), this->snake.length[0]);
		outtextxy(810, 100, myLen);
	}
	else if (snake.mode == MODE::multi) {
		//��ʾ˫��ģʽ�µ�ʱ����ʾ
		settextcolor(RGB(143, 115, 66));
		settextstyle(30, 0, _T("Arial BLACK"));
		setbkmode(TRANSPARENT);
		outtextxy(47, 88, myTime);

		//�����۵�ɫ
		setfillcolor(WHITE);
		solidroundrect(540, 85, 740, 105, 3, 3);//��ɫС�ߵ�������
		solidroundrect(220, 85, 420, 105, 3, 3);//��ɫС�ߵ�������
		//��ӡ������
		for (int i = 0; i < 2; i++) {
			//�����������Ķ���������ʾ��ɫ
			if (this->snake.life[i] <= 10)
				setfillcolor(RGB(205, 51, 51));
			else if (this->snake.life[i] > 10 && this->snake.life[i] <= 50) //��Ϊ��ȫʱ�û�ɫ��ʾ
				setfillcolor(RGB(255, 130, 71));
			else//��ȫ��ȫʱ����ɫ��ʾ
				setfillcolor(RGB(154, 205, 50));
			if (i == 1) {
				if (snake.score[1] <= 660)
					solidroundrect(540, 85, 540 + snake.length[1] * 3, 105, 3, 3);
				else
					solidroundrect(540, 85, 740, 105, 3, 3);
			}//��ӡ��ɫС��������
			else {
				if (snake.score[0] <= 660)
					solidroundrect(420 - snake.length[0] * 3, 85, 420, 105, 3, 3);
				else
					solidroundrect(220, 85, 420, 105, 3, 3);
			}//��ӡ��ɫС��������
		}
		//��ӡ������
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
//����ײǽ
void MANAGER::manager_crashwall() {
	int prespeed[2];
	prespeed[0] = snake.speed[0];
	prespeed[1] = snake.speed[1];
	bool alive = true;
	for (int i = 0; i <= snake.mode; i++) {
		alive = true;
		//��ͷ����
		pos snakepos = this->snake.snake_pos[i][0];

		//��ǰҳ���е�����ǽ�ڶ�Ҫ�����ж�һ��
		for (size_t wallnum = 0; wallnum < this->wall.wall_pos.size(); wallnum++) {
			pos wallpos = this->wall.wall_pos[wallnum];//ĳһ��ǽ�ڵ�����
			int r = snake.radio;//ǽ�ڵĺ�ȵ����ߵİ뾶
			if (snakepos.x >= wallpos.x - r && snakepos.x <= wallpos.x + r
				&& snakepos.y >= wallpos.y - r && snakepos.y <= wallpos.y + r) {
				//������ײǽ�¼�

				if (this->wall.wall_getQuality(wallnum)) {
					int prelength = this->snake.length[i];
					//ײ������ǽ��
					HWND h = GetHWnd();
					MessageBox(h, _T("oops!!!����ײ���ˡ���ǽ�ڡ���\n�㽫�ص�������������Ϸ~\n>>���»س�[ȷ��]������Ϸ"), _T("GAME-����ײǽ��ʾ"), MB_OK);
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
					//ײ��Ӳǽ��С��ֱ����
					HWND h = GetHWnd();
					MessageBox(h, _T("oops!!!����ײ���ˡ�Ӳǽ�ڡ���\n>>���ź�������Ϸ������~"), _T("GAME-����ײǽ��ʾ"), MB_OK);
					this->snake.life[i] = 0;//����ֵ��Ϊ0
					alive = false;
					this->manager_multi_setwinner(i + 1);//����ʤ��
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
//�жϵ�ǰ�����Ϸ�Ƿ�Ҫ����
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
//˫��ģʽ���趨ʤ��
void MANAGER::manager_multi_setwinner(int index) {
	if (index == 1)
		this->winner = 2;
	else if (index == 2)
		this->winner = 1;
	else
		this->winner = 0;
}
//չʾ���а�
void MANAGER::manager_readranking() {
	//�Զ����ƶ��뷽ʽ���ļ�
	ifstream ranking;
	ranking.open("rank.txt", ios::binary);
	if (ranking.is_open()) {//ȷ���ļ���ȷ��
		//��ģʽ
		ranking >> easy_num;
		for (int i = 0; i < easy_num; i++) {
			ranking >> easy_rank[i];//�����û���

			int inlevel = 0;
			ranking >> inlevel;
			level[0][i] = (Level)inlevel;//������Ϸ�Ѷ�

			ranking >> score[0][i];//�������
		}
		//�˻�ģʽ
		ranking >> ai_num;
		for (int i = 0; i < ai_num; i++) {
			ranking >> ai_rank[i];//�����û���

			int inlevel = 0;
			ranking >> inlevel;
			level[1][i] = (Level)inlevel;//������Ϸ�Ѷ�

			ranking >> score[1][i];//�������
		}
		//˫��ģʽ
		ranking >> multi_num;
		for (int i = 0; i < multi_num; i++) {
			ranking >> multi_rank[i];//�����û���

			int inlevel = 0;
			ranking >> inlevel;
			level[2][i] = (Level)inlevel;//������Ϸ�Ѷ�

			ranking >> score[2][i];//�������

		}
		ranking.close();//�ر��ļ�
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

//��ӡ���а�
void MANAGER::manager_renderRanking() {

	//��ȡ�ļ�
	this->manager_readranking();

	//�������Ļ�˵��û���
	TCHAR show_user[15] = { 0 };
	//�������Ļ�˵İ汾
	TCHAR show_edition[15] = { 0 };
	//�������Ļ�˵ķ���
	TCHAR show_score[5] = { 0 };

	//�����ģʽ�����а�
	for (int i = 0; i < easy_num; i++) {
		//�ֽ�����ֽ�ת��
		//char���͵��û���ת��Ϊ���ֽ�
		MultiByteToWideChar(CP_ACP, 0, easy_rank[i], -1, show_user, 15);
		//�ѶȰ汾
		judge_edition(level[0][i], show_edition);

		//����
		_stprintf(show_score, _T("%d"), score[0][i]);

		//���
		settextcolor(RGB(143, 115, 66));
		settextstyle(20, 0, _T("Arial BLACK"));
		setbkmode(TRANSPARENT);
		outtextxy(100, 235 + i * 108, show_user);
		outtextxy(100, 270 + i * 108, show_edition);
		outtextxy(180, 270 + i * 108, show_score);
	}
	//����˻�ģʽ�����а�
	for (int i = 0; i < ai_num; i++) {
		//�ֽ�����ֽ�ת��
		//char���͵��û���ת��Ϊ���ֽ�
		MultiByteToWideChar(CP_ACP, 0, ai_rank[i], -1, show_user, 15);
		//�ѶȰ汾
		judge_edition(level[1][i], show_edition);
		//����
		_stprintf(show_score, _T("%d"), score[1][i]);

		//���
		settextcolor(RGB(143, 115, 66));
		settextstyle(20, 0, _T("Arial BLACK"));
		setbkmode(TRANSPARENT);
		outtextxy(410, 235 + i * 108, show_user);
		outtextxy(410, 270 + i * 108, show_edition);
		outtextxy(490, 270 + i * 108, show_score);
	}
	//���˫��ģʽ�����а�
	for (int i = 0; i < multi_num; i++) {
		//�ֽ�����ֽ�ת��
		//char���͵��û���ת��Ϊ���ֽ�
		MultiByteToWideChar(CP_ACP, 0, multi_rank[i], -1, show_user, 15);
		//�ѶȰ汾
		judge_edition(level[2][i], show_edition);
		//����
		_stprintf(show_score, _T("%d"), score[2][i]);

		//���
		settextcolor(RGB(143, 115, 66));
		settextstyle(20, 0, _T("Arial BLACK"));
		setbkmode(TRANSPARENT);
		outtextxy(720, 235 + i * 108, show_user);
		outtextxy(720, 270 + i * 108, show_edition);
		outtextxy(800, 270 + i * 108, show_score);
	}
}