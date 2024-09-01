#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<graphics.h>
#include<iostream>
#include<algorithm>
#include<time.h>
#include<functional>
#include<vector>
#include<Windows.h>
#include<string.h>
using namespace std;
#define BORDER_WIDTH 960
#define BORDER_HEIGHT 540

//����ҳ���ö������
enum PageType {
	menu_page,//���˵�ҳ��
	easy_game_page,//����-��ģʽ
	multi_game_page,//˫��-�˻�ģʽor˫��ģʽ
	rank_page,//���а�
};
//��Ϸ�Ѷȵ�ö������
enum Level {
	first,//���Ű�
	second,//���װ�
	third//�߼���
};

//��װ��ť
class BUTTON {
private:
	pair<int, int>pos;//��ťλ������
	wstring text;//��ť�ı�
	pair<int, int>size;//��ť��С
	float scale;//���ű���������չʾ�����ͣЧ��
	bool isMouseOver;//����Ƿ��ڰ�ť�Ϸ�
	void(*onClick)();//�����������ʱ��ִ�еĺ���
public:
	//���캯��
	BUTTON(int x, int y, int width, int height, const wstring& info, void(*myFun)());
	//���ư�ť
	void myGUI_draw_BUTTON();
	//�������Ƿ��ڰ�ť�Ϸ�
	void checkMouseOver(int mouseX, int mouseY);
	//���������Ƿ����ڰ�ť�ڲ�ִ�к�����
	bool checkClick(int mouseX, int mouseY);
};

//��װ1��ҳ��
class PAGE {
private:
	int page_width;//ҳ����
	int page_height;//ҳ��߶�
	IMAGE background;//����ͼ
	Level level;//��Ϸ�Ѷ�
	PageType pagetype;//��ǰҳ������
	vector<BUTTON>buttons;//��ǰҳ���ϵ����а�ť
	int button_nums;
	ExMessage msg;//��ҳ�����Ϣ
public:
	wstring bkpaper_name;
public:
	//���캯��
	PAGE(int width, int height, PageType pagetype, wstring paper) :page_width(width), page_height(height), pagetype(pagetype), bkpaper_name(paper) {
		level = Level::first;
		msg = { 0 };
		button_nums = 0;
	}
	//��ʼ����ǰҳ��
	void init_page();
	//�ڵ�ǰҳ����Ӱ�ť��Ϣ
	void add_Button();
	//���ư�ť
	void draw();
	//��������ƶ�
	void mouseMove(int mouseX, int mouseY);
	//���������
	void mouseClick(int mouseX, int mouseY);
	//����ҳ��
	void run();
};