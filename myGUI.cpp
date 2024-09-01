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
	if (MessageBox(h, _T("��ѡ���С��Ƥ��Ϊ��RED"), _T("Ƥ����ɫѡ��"), MB_OK)) {
		manager.snake.set_skincolor(0, redsnake);
		manager.snake.set_skincolor(1, redsnake);
	}
}
void yellow() {
	game_easy.bkpaper_name = name[1];

	HWND h = GetHWnd();
	if (MessageBox(h, _T("��ѡ���С��Ƥ��Ϊ��YELLOW"), _T("Ƥ����ɫѡ��"), MB_OK)) {
		manager.snake.set_skincolor(0, yellowsnake);
		manager.snake.set_skincolor(1, yellowsnake);
	}
}
void blue() {
	game_easy.bkpaper_name = name[2];

	HWND h = GetHWnd();
	if (MessageBox(h, _T("��ѡ���С��Ƥ��Ϊ��BLUE"), _T("Ƥ����ɫѡ��"), MB_OK)) {
		manager.snake.set_skincolor(0, bluesnake);
		manager.snake.set_skincolor(1, bluesnake);
	}
}
//��ͣ����
void myPause() {
	HWND h = GetHWnd();
	MessageBox(h, _T("���ѽ���Ϸ��ͣ������[ȷ��]����Ϸ����"), _T("��Ϸ��ͣ"), MB_OK);
}
//����ģʽ��AI��
void myeasyAI() {
	if (manager.snake.cgtoAI == true)
		manager.snake.cgtoAI = false;//�뿪AIģʽ
	else {
		manager.snake.cgtoAI = true;
		manager.snake.speed[0] = 1;
	}//���ý���AIģʽ
}
//BUTTON�ࣺ���캯��
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
//BUTTON�ࣺ���ư�ť����Ϊ��ͣ�벻��ͣ��
void BUTTON::myGUI_draw_BUTTON() {
	//���ð�ť�Ļ�����Ϣ
	int width = int(size.first * scale);//���ź�ť���
	int height = int(size.second * scale);//���ź�ť�߶�
	int x = pos.first + (size.first - width) / 2;//���ź�ť��X����
	int y = pos.second + (size.second - height) / 2;//���ź�ť��y����

	//���ñ߿�������ɫ
	setlinecolor(RGB(border_line[0], border_line[1], border_line[2]));
	//��������Ƿ���ͣ���ð�ť���ɫ
	if (isMouseOver)
		setfillcolor(RGB(light_button[0], light_button[1], light_button[2]));
	else
		setfillcolor(RGB(normal_button[0], normal_button[1], normal_button[2]));


	setbkmode(TRANSPARENT);//�����ı�����Ϊ͸��
	fillroundrect(x, y, x + width, y + height, 15, 15);
	settextstyle(25, 0, _T("Arial BLACK"), 0, 0, 30, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0);
	settextcolor(WHITE);

	//�����ֻ����ھ�������
	int tx = pos.first + (size.first - textwidth(text.c_str())) / 2;
	int ty = pos.second + (size.second - textheight(text.c_str())) / 2;
	//��ӡ���ı�
	outtextxy(tx, ty, text.c_str());
}
//BUTTON�ࣺ��������ͣ�ڰ�ť�Ϸ�
void BUTTON::checkMouseOver(int mouseX, int mouseY) {
	isMouseOver = ((mouseX >= pos.first && mouseX <= pos.first + size.first)
		&& (mouseY >= pos.second && mouseY <= pos.second + size.second));

	if (isMouseOver)//�����ͣ���Ϸ�ʱ�ߴ���С
		scale = 0.9f;
	else//����ͣʱ���������ߴ�
		scale = 1.0f;
}
//BUTTON�ࣺ�������ڰ�ť�ڲ��������ʱ��
bool BUTTON::checkClick(int mouseX, int mouseY) {
	if ((mouseX >= pos.first && mouseX <= pos.first + size.first)
		&& (mouseY >= pos.second && mouseY <= pos.second + size.second)) {
		isMouseOver = true;
		onClick();//ִ�ж�Ӧ����ָ����ָ��ĺ���
		//BUTTON�ĸ������Իָ�����ͣǰ
		isMouseOver = false;
		scale = 1.0f;
		return true;
	}
	return false;
}


//PAGE�ࣺ��ʼ��ҳ�����
void PAGE::init_page() {
	initgraph(this->page_width, this->page_height, SHOWCONSOLE);
}
//PAGE�ࣺ��Ӱ�ť��Ϣ������
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

		BUTTON easy_mode(630, 440, 85, 35, _T("����ģʽ"), myGame_easy);
		this->buttons.push_back(easy_mode);
		button_nums++;

		BUTTON hard_mode(730, 440, 85, 35, _T("�˻�ģʽ"), myGame_ai);
		this->buttons.push_back(hard_mode);
		button_nums++;

		BUTTON player_mode(830, 440, 85, 35, _T("˫��ģʽ"), myGame_multi);
		this->buttons.push_back(player_mode);
		button_nums++;

		BUTTON ranking(740, 490, 70, 35, _T("���а�"), myRank);//���޸�
		this->buttons.push_back(ranking);
		button_nums++;

		BUTTON helper(830, 490, 85, 35, _T("HELP"), myHelp);
		this->buttons.push_back(helper);
		button_nums++;

		BUTTON quit(850, 20, 85, 35, _T("����˳�"), quitMenu);
		this->buttons.push_back(quit);
		button_nums++;
		break;
	}
	case easy_game_page:
	{
		BUTTON rule(580, 348, 85, 30, _T("����鿴"), myRule);
		this->buttons.push_back(rule);
		button_nums++;

		BUTTON pause(580, 422, 85, 30, _T("�����ͣ"), myPause);
		this->buttons.push_back(pause);
		button_nums++;

		BUTTON ai(580, 493, 85, 30, _T("����л�"), myeasyAI);//����AIģʽ
		this->buttons.push_back(ai);
		button_nums++;

		BUTTON back(20, 15, 95, 30, _T("<������ҳ"), myMenu);
		this->buttons.push_back(back);
		button_nums++;
		break;
	}
	case multi_game_page: {
		BUTTON pause(130, 15, 85, 30, _T("�����ͣ"), myPause);
		this->buttons.push_back(pause);
		button_nums++;

		BUTTON back(20, 15, 95, 30, _T("<������ҳ"), myMenu);
		this->buttons.push_back(back);
		button_nums++;
		break;
	}
	case rank_page: {
		BUTTON back(20, 15, 95, 30, _T("<������ҳ"), myMenu);
		this->buttons.push_back(back);
		button_nums++;
		break;
	}
	}
}
void PAGE::draw() {
	//���ر���ͼ
	loadimage(&background, bkpaper_name.c_str(), this->page_width, this->page_height);
	putimage(0, 0, &background);
	//���ư�ť
	for (int index = 0; index < button_nums; index++)
		buttons[index].myGUI_draw_BUTTON();
}
//������굥���¼�
void PAGE::mouseClick(int mouseX, int mouseY) {
	for (int index = 0; index < button_nums; index++) {
		if (buttons[index].checkClick(mouseX, mouseY))
			break;
	}
}
//��������ƶ��¼�
void PAGE::mouseMove(int mouseX, int mouseY) {
	for (int index = 0; index < button_nums; index++) {
		buttons[index].checkMouseOver(mouseX, mouseY);
	}
}

void PAGE::run() {
	BeginBatchDraw();//��ʼ��������
	while (true) {
		if (peekmessage(&msg, EM_KEY | EM_MOUSE)) {
			switch (msg.message) {
			case WM_KEYDOWN://���̰����¼�
				if (manager.snake.cgtoAI == false && manager.snake.mode == MODE::easy || manager.snake.mode == MODE::multi)
					manager.manager_event(msg.vkcode);
				break;
			case WM_LBUTTONDOWN://�����������¼�
				mouseClick(msg.x, msg.y);//������굥��
				break;
			case WM_MOUSEMOVE://����ƶ��¼�
				mouseMove(msg.x, msg.y);//��������ƶ��¼�
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
		FlushBatchDraw();//��������������ʾ����Ļ��
	}
	EndBatchDraw();//������������
}

