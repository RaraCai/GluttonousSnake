#pragma once
#include"item.h"

//�ۺϹ���ģ��
class MANAGER {
private:
	int time;//ÿ����Ϸ�ļ�ʱ���
public:
	SNAKE snake;//����ҳ���е�������
	FOOD food;//����ҳ���е�����ʳ��
	WALL wall;//����ҳ���е������ϰ���
	wchar_t user1[15];//���������ʾ����һ���ߵ��û���
	wchar_t user2[15];//���������ʾ���ڶ����ߵ��û���
	int winner;//���ÿһ�ֵ���ϷӮ��
	bool checkRank;
private:
	void manager_draw_info();//��Ϸ�����е������Ϣ
	void manager_readranking();//��ȡ�ļ��д洢�����а���Ϣ
public:
	MANAGER();//���캯��
	void manager_update();//����״̬
	void manager_ai();//������Ϸ�µļ���AIģʽ
	void manager_render();//��Ⱦ
	void manager_event(BYTE& vkcode);//�¼�����
	void manager_eatfood();//��ʳ�����ز���
	void manager_reset_time();//��Ϸʱ������
	void manager_crashwall();//ײǽ����ز���
	bool manager_isRunning();//��鵱����Ϸ�Ƿ�Ҫ����
	void manager_multi_setwinner(int index);//˫��ģʽ���趨ʤ��
	void manager_renderRanking();//��ӡ���а�
};