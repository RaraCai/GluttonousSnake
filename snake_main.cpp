#include"myGUI.h"
#include"page.h"

MANAGER manager;


int main() {

	//������ɺ���������
	srand((unsigned int)time(NULL));
	//��ҳ��GUI�ĳ�ʼ��
	menu.init_page();
	menu.add_Button();
	game_easy.add_Button();
	game_multi.add_Button();
	game_ai.add_Button();
	ranking.add_Button();
	//���Ʋ˵�����
	menu.draw();
	//��ҳ��ĵ���
	HWND h = GetHWnd();
	MessageBox(h, _T("��ӭ�������ߵ����磡\n����ѡ�񡾵���ģʽ��:\n>>����ѡ����ϲ����С�ߡ�Ƥ������\n����ѡ���˻�ģʽ����˫��ģʽ��:\n>>�����½�㻹û��Ϊ�㿪����ѡƤ����Ȩ��~\n[HELP]:����������ڡ�HELP�����鿴~"), _T("�˵�ҳ-NOTICE"), MB_OK);
	//����˵����濪ʼ����
	menu.run();
}