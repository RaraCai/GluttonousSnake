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


//�˵���������
void myMenu();
//д���ļ���¼��������
void write_record(PageType pagetype, MODE mode, Level level, char user1[], int score1, int score2 = 0, char user2[] = (char*)"AI_snake");

//����ģʽ���˳���Ϸҳ��
void easy_exit() {
	HWND h = GetHWnd();
	if (MessageBox(h, _T("��Ϸ����!\n��ȷ�������ز˵�"), _T("��Ϸ��ʾ"), MB_OK)) {
		//������Ҫ����һ���ļ��е���ʷ��¼
		char user1[15] = { 0 };
		sprintf_s(user1, "%ws", manager.user1);
		write_record(PageType::easy_game_page, manager.snake.mode, manager.snake.level, user1, manager.snake.score[0]);
		//�������а�
		easy_num++;
		update_rank(manager.snake.score[0], user1, manager.snake.level, 1);
		write_rank();
		//��״̬��ֱ�����õ�ǰ�ߵĸ�������Ϊ��ʼ״̬�������浱ǰ��ͼ
		manager.food.food_set(manager.snake, manager.wall);
		manager.snake.snake_reset(0);
		manager.wall.wall_reset();
		myMenu();
	}

}
//�����ߵ�ģʽ���˳�
void multi_exit() {
	HWND h = GetHWnd();
	if (manager.snake.cgtoAI == false) {//˫��ģʽ
		if (manager.winner == 1)//player1 win
			MessageBox(h, _T("����Ӯ�ң�Player1"), _T("��Ϸ��ʾ"), MB_OK);
		else if (manager.winner == 2)//player2 win
			MessageBox(h, _T("����Ӯ�ң�Player2"), _T("��Ϸ��ʾ"), MB_OK);
		else //win equal
			MessageBox(h, _T("����ƽ�֣�"), _T("��Ϸ��ʾ"), MB_OK);
		multi_num += 2;
		//multi_max = max(manager.snake.score[0], manager.snake.score[1]);
	}
	else {//�˻���սģʽ
		if (manager.winner == 1)//player1 win
			MessageBox(h, _T("����Ӯ�ң�Player"), _T("��Ϸ��ʾ"), MB_OK);
		else if (manager.winner == 2)//AI player2 win
			MessageBox(h, _T("����Ӯ�ң�AI-Player"), _T("��Ϸ��ʾ"), MB_OK);
		else {//win equal
			MessageBox(h, _T("����ƽ�֣�"), _T("��Ϸ��ʾ"), MB_OK);
			ai_num++;
		}
		ai_num++;
	}

	//������Ҫ����һ���ļ��е���ʷ��¼
	char user1[15] = { 0 };
	char user2[15] = { 0 };
	//sprintf_s(user1, "%ws", manager.user1);
	//sprintf_s(user2, "%ws", manager.user2);

	WideCharToMultiByte(CP_ACP, 0, manager.user1, -1, user1, 15, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, manager.user2, -1, user2, 15, NULL, NULL);

	write_record(PageType::easy_game_page, manager.snake.mode, manager.snake.level, user1, manager.snake.score[0], manager.snake.score[1], user2);

	//�������а�
	//˫��ģʽ���˻�ģʽ��ͬʱ����������ҵ���Ϸ��¼������AI�ߵļ�¼
	update_rank(manager.snake.score[0], user1, manager.snake.level, 3 - manager.snake.cgtoAI);
	update_rank(manager.snake.score[1], user2, manager.snake.level, 3 - manager.snake.cgtoAI);
	write_rank();//д�����а��ļ�

	//��״̬��ֱ�����õ�ǰ�ߵĸ�������Ϊ��ʼ״̬�������浱ǰ��ͼ
	manager.food.food_set(manager.snake, manager.wall);
	manager.wall.wall_reset();
	manager.snake.snake_reset(0);
	manager.snake.snake_reset(1);//˫�˶�ս�ڶ�����Ҳ��Ҫ����
	//�������˵�
	myMenu();

}

//����ģʽ��˫��ģʽ�µ�ͬ�˳����
void SNAKE::way_to_exit() {
	if (this->mode == MODE::easy) {
		easy_exit();
	}
	else if (this->mode == MODE::multi) {
		multi_exit();
	}
}
//3���汾����ײ���߽�ʱ�Ĳ�ͬ����
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



//��ʼ����
SNAKE::SNAKE() {
	this->cgtoAI = false;
	//�ȳ�ʼ����Ÿ�����ͷ�������ά��vector
	snake_pos.resize(2);
	this->radio = 5;
	//2���ߵ�Ĭ����ɫ
	this->color[0] = 0; color[1] = 2;
	this->set_skincolor(0, color[0]);
	this->set_skincolor(1, color[1]);
	for (int index = 0; index < 2; index++) {
		this->direction[index] = DIR::right;//��ʼ����right
		this->length[index] = 3;//��ʼ����Ϊ3
		this->score[index] = 0;//��ʼ�÷�
		this->speed[index] = 1;
		this->life[index] = 10;//һ�����壨������ͷ��5������ֵ
		//��ʼ��ͷλ�÷���vector
		pos begin = { 300 + index * 100,300 + index * 100,skin_color[this->color[index]] };
		snake_pos[index].push_back(begin);


		//��ʼ����������ڵ���ɫ
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
//��Ⱦ�ߵ��۾�
void SNAKE::snake_eyeRender(int x, int y) {
	setfillcolor(WHITE);
	solidcircle(x, y, 3);
	setfillcolor(BLACK);
	solidcircle(x, y, 2);
	setfillcolor(WHITE);
	solidcircle(x + 1, y - 1, 1);
}
//��Ⱦ�ߵ�����
void SNAKE::snake_bodyRender(int index) {

	for (int i = this->length[index] - 1; i >= 0; i--) {
		setfillcolor(this->snake_pos[index][i].color);
		solidcircle(snake_pos[index][i].x, snake_pos[index][i].y, this->radio);
	}
	//���۾�����Ե�ǰ��ͷ��������������
	int eyewidth = 4;//����֮��Ŀ��
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
//�����ߵ�Ƥ����ɫ
void SNAKE::set_skincolor(int index, int color) {
	this->color[index] = color;
	for (int i = 0; i < this->length[index]; i++) {
		if (i % 2 == 0)
			this->snake_pos[index][i].color = skin_color[color];
	}
}
//�ߵ��ƶ�ģ��
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


	//��һ����λ����ǰһ�����ϴε�λ��
	for (int i = this->length[index] - 1; i > 0; i--) {
		snake_pos[index][i].x = snake_pos[index][i - 1].x;
		snake_pos[index][i].y = snake_pos[index][i - 1].y;
	}
	//���ݷ������ƶ�
	switch (this->direction[index]) {
	case DIR::up:
		snake_pos[index][0].y -= this->speed[index];
		if (snake_pos[index][0].y <= up_limit)
		{/*����������Ϸģʽ������*//*ֱ���������Ǳ��ʳ���ǽ*/
			snake_invalid(index);
			crashtimes[index]++;
		}
		break;
	case DIR::down:
		snake_pos[index][0].y += this->speed[index];
		if (snake_pos[index][0].y >= down_limit)
		{/*����������Ϸģʽ������*//*ֱ���������Ǳ��ʳ���ǽ*/
			snake_invalid(index);
			crashtimes[index]++;
		}
		break;
	case DIR::left:
		snake_pos[index][0].x -= this->speed[index];
		if (snake_pos[index][0].x - this->radio <= left_limit)
		{/*����������Ϸģʽ������*//*ֱ���������Ǳ��ʳ���ǽ*/
			snake_invalid(index);
			crashtimes[index]++;
		}
		break;
	case DIR::right:
		snake_pos[index][0].x += this->speed[index];
		if (snake_pos[index][0].x + this->radio >= right_limit)
		{/*����������Ϸģʽ������*//*ֱ���������Ǳ��ʳ���ǽ*/
			snake_invalid(index);
			crashtimes[index]++;
		}
		break;
	}
}
//�ߵ��¼�
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
			if (this->speed[index] == 1)//���ٴ���
				this->speed[index] = 3;
			else if (this->speed[index] == 3)
				this->speed[index] = 1;//�Ѿ����ڼ���״̬���ٴΰ��ո�ԭ�س�ʼ�ٶ�
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
			if (this->speed[index] == 1)//���ٴ���
				this->speed[index] = 3;
			else if (this->speed[index] == 3)
				this->speed[index] = 1;//�Ѿ����ڼ���״̬���ٴΰ��ո�ԭ�س�ʼ�ٶ�
			break;
		}
	}
}
//�ߵ�����
void SNAKE::snake_increase(int index, int increase) {
	////����֤���÷֣���Ϸ��ʾ��
	//mciSendString(_T("close incorrect"), NULL, 0, NULL);
	//mciSendString(_T("open ./increase.mp3 alias incorrect"), NULL, 0, NULL);
	//mciSendString(_T("play incorrect"), NULL, 0, NULL);
	//��������������ɫ����ʼ��
	for (int i = this->length[index]; i < length[index] + increase; i++) {
		pos incre_section = { 0 };
		if (i % 2 == 0)
			incre_section.color = skin_color[this->color[index]];
		else
			incre_section.color = WHITE;
		this->snake_pos[index].push_back(incre_section);
	}
	//�ı�������
	this->length[index] += increase;// �Ժù��ӵ÷�
	//�ı䵱ǰ�ߵĵ÷�
	this->score[index] += increase * 10;
	//�ı䵱ǰ�ߵ�������
	this->life[index] += increase * 5;
}
//�ߵļ���
void SNAKE::snake_decrease(int index, int decrease) {
	//�жϼ��ٵĳ����Ƿ�Ϸ�
	if (decrease >= this->length[index]) {
		this->life[index] = 0;
		this->score[index] -= decrease * 10;
		return;
	}
	//�����ȼ��ٺ��߻��ܴ����vector�����һ��Ԫ�ؿ�ʼ�����ȼ���
	for (int i = 0; i < decrease; i++)
		this->snake_pos[index].pop_back();
	this->length[index] -= decrease;
	this->life[index] -= decrease * 5;//�Ի����Ӳ��÷� ������
}
//�����ߵĸ�������
void SNAKE::snake_reset(int index) {
	//������������ɫ
	this->color[0] = 0; color[1] = 2;
	this->set_skincolor(0, color[0]);
	this->set_skincolor(1, color[1]);
	//�÷��޸�Ϊ0
	this->score[index] = 0;
	snake_pos.resize(2);
	//�������Ԫ�أ������ûس�ʼ����
	this->snake_pos[index].clear();
	this->length[index] = 3;//��������س�ʼ����=3
	//������ͷλ�ü�������ڵ�λ��
	//��ʼ��ͷλ�÷���vector
	pos begin = { 300 + index * 100,300 + index * 100,skin_color[this->color[index]] };
	snake_pos[index].push_back(begin);
	//��ʼ����������ڵ���ɫ
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
	//�ߵ��ٶ��޸Ļس�ʼ�ٶ�
	this->speed[index] = 1;
	//�Ƿ����AIģʽ�޸Ļ�false
	this->cgtoAI = false;
	//�ߵ�����ֵ�޸Ļس�ʼ״̬
	this->life[index] = 10;
}
//����ߵ�����״̬
bool SNAKE::snake_checklife(int index) {
	//�ߵ�����ֵ��Ϊ0�����£�thirdģʽ��ײǽ��5�Σ�ͷβ��ӣ���Ϸ�������ж�����
	if (this->life[index] <= 0 || (level == Level::third && crashtimes[index] > 5) ||
		(snake_pos[index][0].x == snake_pos[index][length[index] - 1].x && snake_pos[index][0].y == snake_pos[index][length[index] - 1].y))
	{
		//���ñ�����Ϸ��ʤ����
		manager.manager_multi_setwinner(index + 1);
		////�������
		//manager.snake.score[index] = 0;
		if (level == Level::third && crashtimes[index] > 5) {
			HWND h = GetHWnd();
			MessageBox(h, _T("Sorry!!�ڵ�ǰģʽ��ײǽ����5�ξͻ������ϷŶ~"), _T("��Ϸ��ʾ"), MB_OK);
		}
		return false;//�ߵ��˸�����ʱ��ͷ���false
	}
	else if (mode == MODE::multi && abs(score[0] - score[1]) > 200) {
		//������Ϸʤ��
		int winner = (score[0] > score[1]) ? 1 : 0;
		manager.manager_multi_setwinner(winner + 1);
		return false;
	}
	else
		return true;//С�߻�����
}
//��ģʽ�µ�AI��
void SNAKE::snake_easyAI(int index, pos target) {
	//��ȡ��ǰ����ͷ����
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

	//����ͷ������Ŀ������Ƚ�
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




/*******************************FOOD���Ա������ʵ��*************************************/

//�ж�ʳ�ﵱǰ״̬
bool FOOD::food_getAlive(int index) {
	return this->alive[index];//��û���Ե�����true
}
//����ʳ��
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
//��ʼ���������õ�ǰҳ�����й��ӵ�״̬
void FOOD::food_set(SNAKE& snake_group, WALL& wall) {

	int begin_x = (manager.snake.mode == MODE::easy) ? GAME_BEGIN_X : GAME_MULTI_BEGIN_X;
	int begin_y = (manager.snake.mode == MODE::easy) ? GAME_BEGIN_Y : GAME_MULTI_BEGIN_Y;
	int width = (manager.snake.mode == MODE::easy) ? GAME_WIDTH : GAME_MULTI_WIDTH;
	int height = (manager.snake.mode == MODE::easy) ? GAME_HEIGHT : GAME_MULTI_HEIGHT;

	//�Ƚ����й��ӵ�alive���Զ���Ϊfalse
	for (int i = 0; i < 50; i++)
		this->alive[i] = false;
	//����������ɳ�ʼʱ�Ĺ�������
	this->sum = rand() % 5 + 1;
	this->left = 0;//�ù��ӵĳ�ʼ����Ϊ0�����ݺ������ɵĹ������Լ���
	//�������ù��ӵ�����
	for (int i = 0; i < this->sum; i++) {
		//�������й��ӵ����Զ�Ϊû���Ե�
		this->alive[i] = true;
		//������ɹ��ӵķ���
		this->score[i] = rand() % 3 + 5;
		//���������Щ���ӵĺû�����
		this->good_bad[i] = rand() % 2;//1-true-good,0-false-bad
		//�������ɺù��ӣ����������1
		if (good_bad[i])
			this->left++;
		//�����ݺû��������ù��ӵ���ɫ
		if (good_bad[i] == true)//good
			this->color[i] = good_food_color[rand() % 5];
		else//bad
			this->color[i] = RGB(139, 137, 112);
		//������ɵ�ǰ���ӵ�λ������
		bool valid = true;
		while (true) {
			valid = true;
			food_pos[i].x = rand() % (width - 40) + begin_x + 20;
			food_pos[i].y = rand() % (height - 40) + begin_y + 20;
			//�����ɵ�λ��������һ���Ѿ��е�ʳ���غϣ�Ҳ��Ҫ��������
			while (this->food_checkpos(i) == false)
			{
				food_pos[i].x = rand() % (width - 40) + begin_x + 20;
				food_pos[i].y = rand() % (height - 40) + begin_y + 20;

			}

			//ȷ�������ɵĹ��Ӳ����κ�һ��ǽ��
			while (wall.wall_checkpos(food_pos[i].x, food_pos[i].y) == false) {
				food_pos[i].x = rand() % (width - 40) + begin_x + 20;
				food_pos[i].y = rand() % (height - 40) + begin_y + 20;
			}

			//ȷ�������ɵĹ���λ�ò����κ�һ���ߵ�����
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
		}//��������ȷ��ʳ������
	}
}
//����ĳһ��ʳ��Ĵ��״̬
void FOOD::food_setAlive(int index, bool setAlive) {
	this->alive[index] = setAlive;
}
//��ȡĳһ��ʳ��ķ���
int FOOD::food_getScore(int index) {
	return this->score[index];
}
//�ж�ĳһ��ʳ��ĺû�����
bool FOOD::food_getQuality(int index) {
	return this->good_bad[index];//�ù��ӷ���true
}
//�жϵ�ǰ���ɵ����λ�û᲻�������е�ʳ���ص��������ص�˵��λ����Ч����true
bool FOOD::food_checkpos(int food_index) {
	for (int i = 0; i < food_index; i++) {
		if (this->food_pos[food_index].x == food_pos[i].x && this->food_pos[food_index].y == food_pos[i].y)
			return false;
	}
	return true;
}
//��ײǽ���ߵ�β���������ʳ��
void FOOD::food_Cgbysnake(vector<pos>& snake_pos) {
	if (this->sum >= 50)
		return;
	//����ײǽ��λ��������1~3��ʳ��
	int length = snake_pos.size();
	int newpos[3] = { -1,-1,-1 };
	//������С��3����Ĭ�ϱ��ǽ
	if (length > 3) {
		//���ߵ�������3�����λ��
		for (int i = 0; i < 3; i++) {
			newpos[i] = rand() % (length - 1) + 1;
			for (int j = i - 1; j >= 0; j--) {
				while (newpos[j] == newpos[i])
					newpos[i] = rand() % length;
			}//�ҵ�������ͬ��3��λ��
			this->alive[sum + i] = true;//��û���Ե�
			this->score[sum + i] = rand() % 3 + 5;//�������
			this->good_bad[sum + i] = rand() % 2;//�û�����
			//���ݺû�����������ɫ
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

/*******************************WALL���Ա������ʵ��*************************************/
WALL::WALL() {
	this->wall_num = 0;
	this->wall_radio = 5;
}
//��ԭ���������ߵ�λ�����������ǽ�ں�Ӳǽ��
void WALL::wall_build(vector<pos>dead_snake_pos) {
	//���ȷ���������ɵ�ǽ����ǽ�ڻ���Ӳǽ��
	bool soft_hard = rand() % 2;
	int num = (int)dead_snake_pos.size();//ȷ��һ���������ٿ�ǽ��
	for (int i = 0; i < num; i++) {
		this->wall_num++;
		this->wall_pos.push_back(dead_snake_pos[i]);
		this->wall_soft_hard.push_back(soft_hard);
		//������Ӳǽ�ھ�����ɫ����
		if (soft_hard) {//soft_hard=trueΪ��ǽ��
			wall_color.push_back(RGB(217, 217, 217));
		}
		else {//Ӳǽ�ڵ���ɫ����ǽ����ɫ����
			wall_color.push_back(RGB(166, 166, 166));
		}
	}
}

//��������ǽ��
void WALL::wall_render() {
	for (int i = 0; i < wall_num; i++) {
		setfillcolor(this->wall_color[i]);
		int x = this->wall_pos[i].x - wall_radio;
		int y = this->wall_pos[i].y - wall_radio;
		solidroundrect(x, y, x + 2 * wall_radio, y + 2 * wall_radio, 3, 3);
	}
}
//�жϵ�ǰǽ������ǽ�ڻ���Ӳǽ��
bool WALL::wall_getQuality(int wall_index) {
	return this->wall_soft_hard[wall_index];
}
//��������ɵ�ʳ���Ƿ��������ǽ����
bool WALL::wall_checkpos(int food_x, int food_y) {
	for (int i = 0; i < (int)this->wall_pos.size(); i++) {
		if (food_x == this->wall_pos[i].x && food_y == wall_pos[i].y)
			return false;
	}
	return true;
}

//����ǽ������
void WALL::wall_reset() {
	this->wall_num = 0;
	this->wall_soft_hard.clear();
	this->wall_color.clear();
	this->wall_pos.clear();
}