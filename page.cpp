#include"myGUI.h"
#include"manager.h"

extern MANAGER manager;

wstring name[4] = { _T("bk_easy_red.png"),_T("bk_easy_yellow.png"),_T("bk_easy_blue.png"),_T("bk_multi.png") };
PAGE menu(960, 540, menu_page, _T("bk_paper.png"));
PAGE game_easy(960, 540, easy_game_page, name[0]);
PAGE game_multi(960, 540, multi_game_page, name[3]);
PAGE game_ai(960, 540, multi_game_page, _T("bk_ai.png"));
PAGE ranking(960, 540, rank_page, _T("bk_rank.png"));

//�˵�����
void myMenu() {

	bool ai = manager.snake.cgtoAI;
	bool notset = (manager.snake.mode == MODE::multi) && ai;

	//�ص��˵�����˫��ģʽ��Ai����
	manager.snake.cgtoAI = false;
	//�Ե�ǰ��ͼ����մ���
	manager.food.food_set(manager.snake, manager.wall);
	manager.wall.wall_reset();
	for (int i = 0; i < manager.snake.mode; i++)
		manager.snake.snake_reset(i);

	//������ʾ
	HWND h = GetHWnd();
	if (MessageBox(h, _T("�㼴��������ҳ��\n>>[ȷ��]-������ҳ [ȡ��]-�����ڵ�ǰҳ��"), _T("GAME-��Ϸ��ʾ"), MB_OKCANCEL) == IDOK) {
		//�鿴���а�ı������Ϊfalse
		manager.checkRank = false;
		menu.draw();
		menu.run();
	}
	else {
		if (notset)
			manager.snake.cgtoAI = ai;
	}
}
//һ���ߵĽ���
void myGame_easy() {
	manager.snake.mode = easy;
	manager.snake.crashtimes[0] = manager.snake.crashtimes[1] = 0;
	wchar_t level[2] = { 0 };
	InputBox(level, 2, _T("��ӭ�������ߵ����磡\n[GUIDE]��������Ϸ�Ѷȣ�\n  [1]-���Ű�\n  [2]-���װ�\n  [3]-�߼���"), _T("��ģʽ-�Ѷ�ѡ��"));
	//���뵥��ģʽ�µ��û���
	wchar_t user[15] = { 0 };
	InputBox(user, 15, _T("�����ߵ���������������ְɣ�\n>>�����뱾����Ϸ��Ҫʹ�õ��û���:\n>>����ֻ��ʶ��Ӣ�ġ����벻Ҫ����15���ַ�~"), _T("��ģʽ-�û�������"));
	wcscpy(manager.user1, user);
	//������Ϸ�Ѷ�
	if (level[0] == '1')
		manager.snake.level = Level::first;
	else if (level[0] == '2')
		manager.snake.level = Level::second;
	else if (level[0] == '3')
		manager.snake.level = Level::third;
	//���ƽ���
	game_multi.draw();
	//������Ϸʱ��
	manager.manager_reset_time();

	game_easy.run();

}
//˫�˶�ս����
void myGame_multi() {
	//������Ϣ����
	manager.snake.mode = multi;
	manager.snake.crashtimes[0] = manager.snake.crashtimes[1] = 0;

	//����˫��ģʽ�µ��û���
	wchar_t user1[16] = { 0 }, user2[16] = { 0 };
	InputBox(user1, 16, _T("�����ߵ���������������ְɣ�\n>>�����뱾����Ϸ��Player1�����û���:\n>>����ֻ��ʶ��Ӣ�ġ����벻Ҫ����15���ַ�~"), _T("˫��ģʽ-�û�������"));
	wcscpy(manager.user1, user1);
	InputBox(user2, 16, _T("�����ߵ���������������ְɣ�\n>>�����뱾����Ϸ��Player2�����û���:\n>>����ֻ��ʶ��Ӣ�ġ����벻Ҫ����15���ַ�~"), _T("˫��ģʽ-�û�������"));
	wcscpy(manager.user2, user2);

	//������Ϸ�Ѷ�
	wchar_t level[2] = { 0 };
	InputBox(level, 2, _T("BATTLE����������Ҫ��ս�����Ѷȣ�\n[GUIDE]��������Ϸ�Ѷȣ�\n  [1]-���Ű�\n  [2]-���װ�\n  [3]-�߼���"), _T("˫��ģʽ-�Ѷ�ѡ��"));
	if (level[0] == '1')
		manager.snake.level = Level::first;
	else if (level[0] == '2')
		manager.snake.level = Level::second;
	else if (level[0] == '3')
		manager.snake.level = Level::third;

	//���ƽ���
	game_multi.draw();

	//������Ϸʱ��
	manager.manager_reset_time();
	game_multi.run();
}
//�˻���ս����
void myGame_ai() {
	//������Ϣ����
	manager.snake.mode = multi;
	manager.snake.crashtimes[0] = manager.snake.crashtimes[1] = 0;
	//����AI������Ϣ
	manager.snake.cgtoAI = true;
	//�����˻���սģʽ�µ��û���
	wchar_t user1[15] = { 0 };
	InputBox(user1, 15, _T("�����ߵ���������������ְɣ�\n>>�����뱾����Ϸ��Player�����û���:\n>>����ֻ��ʶ��Ӣ�ġ����벻Ҫ����15���ַ�~"), _T("�˻�ģʽ-�û�������"));
	wcscpy(manager.user1, user1);
	wcscpy(manager.user2, _T("AI-Player"));

	//������Ϸ�Ѷ�
	wchar_t level[2] = { 0 };
	InputBox(level, 2, _T("BATTLE��������Ҫ��ս�����Ѷȣ�\n[GUIDE]��������Ϸ�Ѷȣ�\n  [1]-���Ű�\n  [2]-���װ�\n  [3]-�߼���"), _T("�˻�ģʽ-�Ѷ�ѡ��"));
	if (level[0] == '1')
		manager.snake.level = Level::first;
	else if (level[0] == '2')
		manager.snake.level = Level::second;
	else if (level[0] == '3')
		manager.snake.level = Level::third;

	//���ƽ���
	game_ai.draw();

	//������Ϸʱ��
	manager.manager_reset_time();
	game_ai.run();
}
//��������
void myHelp() {
	//��������ĵ���
	HWND h = GetHWnd();
	MessageBox(h, _T("������ģʽ��:\n>>�������ţ��Ӽ�ģʽ��ʼ�ɣ�[1]���Ű� [2]���װ� [3]�߼���\n���˻�ģʽ��:\n>>��Ҫ��ս����������������˻�ģʽ��AI_snakeһ��battle��~\n��˫��ģʽ��:\n>>����������ſ��ԣ�������˫��ģʽ��\n\n>>������ڡ����а��в鿴��Ĺ�����ʷ�ɼ�Ŷ��"), _T("HELP����"), MB_OK);
}
//�˳��˵�����
void quitMenu() {
	closegraph();
	exit(0);
}
//�������
void myRule() {
	//�������ĵ���
	HWND h = GetHWnd();
	if (manager.snake.level == Level::first)
		MessageBox(h, _T("�����Ű桿��Ϸ����\n>>ʹ�á������������С���ƶ�\n>>���¡��ո�С�߼��٣��ٴΰ��¡��ո񡿻ָ�ԭ��\n>>����ɫ����ʵ�ӷ֣�����ɫ����ʵʧ��\n>>ÿ��ֻ�н����С���ɫ����ʵ�Թ⣬�Ż�����¹�ʵŶ��\n>>С�Ŀ������ߣ�һ����ײǽ����Ϸ��������\n>>����ġ������ۡ��򡾷�����Ϊ0ʱ����ϷҲ��������"), _T("GAME-��Ϸ����"), MB_OK);
	else if (manager.snake.level == Level::second)
		MessageBox(h, _T("�����װ桿��Ϸ����\n>>ʹ�á������������С���ƶ�\n>>���¡��ո�С�߼��٣��ٴΰ��¡��ո񡿻ָ�ԭ��\n>>����ɫ����ʵ�ӷ֣�����ɫ����ʵʧ��\n>>ÿ��ֻ�н����С���ɫ����ʵ�Թ⣬�Ż�����¹�ʵŶ��\n>>С�Ŀ������ߣ�һ����ײǽ��������ɡ�ǽ�ڡ������ص���ʼλ�����¿�ʼ��\n>>��ǳ��ɫ��Ϊ����ǽ�ڡ����ᵼ����ʧȥһ��������\n>>�����ɫ��Ϊ��Ӳǽ�ڡ�����ֱ�ӽ�����Ϸ��\n>>����ġ������ۡ��򡾷�����Ϊ0ʱ����ϷҲ��������"), _T("GAME-��Ϸ����"), MB_OK);
	else if (manager.snake.level == Level::third)
		MessageBox(h, _T("���߼��桿��Ϸ����\n>>ʹ�á������������С���ƶ�\n>>���¡��ո�С�߼��٣��ٴΰ��¡��ո񡿻ָ�ԭ��\n>>����ɫ����ʵ�ӷ֣�����ɫ����ʵʧ��\n>>ÿ��ֻ�н����С���ɫ����ʵ�Թ⣬�Ż�����¹�ʵŶ��\n>>С�Ŀ������ߣ�һ����ײǽ��������ɡ���ʵ�������ص���ʼλ�����¿�ʼ��\n>>��ײǽ����������5�Ρ�����Ϸ��������\n>>����ġ������ۡ��򡾷�����Ϊ0ʱ����ϷҲ��������"), _T("GAME-��Ϸ����"), MB_OK);
}
//���а����
void myRank() {
	manager.checkRank = true;
	ranking.draw();
	ranking.run();
}