#pragma once
#include"myGUI.h"

const COLORREF skin_color[3] = { RGB(238,92,66),RGB(255,165,0),RGB(0,154,205) };

typedef struct pos {
	int x;//λ��X����
	int y;//λ��Y����
	COLORREF color;//��ǰλ�õ���ɫ
}pos;
//ö�ٳ�4�ֿ��ܵķ���
enum DIR {
	up, down, left, right
};
enum MODE {
	easy,//����ģʽ��ҳ����ֻ��һ����
	multi//˫��ģʽ��ҳ������2���ߣ������˻���ս��˫��ģʽ
};


class SNAKE {
private:
	void snake_invalid(int index);
public:
	Level level;//��ǰС����������Ϸ�Ѷȣ���������Ĺ����趨
	MODE mode;//����ҳ������һ���߻���������
	int color[2];//С�ߵ�Ƥ����ɫ
	int length[2];//�ߵĳ���
	enum DIR direction[2];//�ߵķ���
	int radio;//����İ뾶
	int speed[2];//�ߵ�ǰ���ٶ�
	vector<vector<pos>>snake_pos;//ÿһ�����������
	int score[2];//�����ߵ�ǰ�õ����ܷ�
	int life[2];//����ֵ
	int crashtimes[2];//ײǽ����
	bool cgtoAI;//��ģʽ���Ƿ��л�AIģʽ

public:
	SNAKE();//���캯��
	void snake_eyeRender(int x, int y);//�ߵ��۾�����
	void snake_bodyRender(int index);//�ߵ��������
	void set_skincolor(int index, int color);//����Ƥ����ɫ
	void snake_move(int index);//�ߵ��ƶ�
	void snake_event(int inedx, BYTE bkcode);//�ߵ��¼�����
	void snake_increase(int index, int increase);//�ߵĳ�������
	void snake_decrease(int index, int decrease);//�ߵĳ�������
	void way_to_exit();//��ͬ��Ϸģʽ�µ��˳���ʽ
	void snake_reset(int index);//�����ߵĸ�������
	bool snake_checklife(int index);//����ߵĴ��״̬
	void snake_easyAI(int index, pos target);//�����ߵ�AIģʽ�㷨
};

class WALL;

class FOOD {
private:
	bool good_bad[50];//�����Ǻù��ӻ��ǻ�����
	int score[50];
	bool alive[50];//�����Ƿ񱻳Ե�
public:
	COLORREF color[50];//ʳ�����ɫ���ù���-��ɫ��������-��ɫ
	int sum;//ÿһ�εĹ�������
	int radio;//���ư뾶
	pos food_pos[50];//ʳ�������
	int left;//ʣ��Ļ�û���Ե��Ĺ���
public:
	FOOD() {};//Ĭ�Ϲ��캯��
	void food_set(SNAKE& snake_group, WALL& wall);//�������й��ӵ����ԣ����캯�����ã����ú�������set����
	void food_render();//������Ⱦ
	bool food_getAlive(int index);//�ж�ʳ����״̬
	void food_setAlive(int index, bool setAlive);//����ʳ����״̬
	int food_getScore(int index);//��ȡʳ���Ӧ����
	bool food_getQuality(int index);//��ȡʳ��û�����
	bool food_checkpos(int food_index);//��鵱ǰ���ɵ�ʳ���Ƿ�������ʳ���غ�
	void food_Cgbysnake(vector<pos>& snake_pos);//��ײǽ����ߵ�β���������ʳ��
};



//�ϰ�����
class WALL {
private:
	int wall_num;//ǽ������
	int wall_radio;
	vector<bool>wall_soft_hard;//��ǽ�ں�Ӳǽ��
	vector<COLORREF>wall_color;//��ǽ�ں�Ӳǽ��
public:
	vector<pos>wall_pos;//���˱߿����⣬�����ϰ����λ��
public:
	WALL();//���캯��
	void wall_build(vector<pos>dead_snake_pos);//�����������߱��ǽ��
	void wall_render();//��������ǽ��
	bool wall_getQuality(int wall_index);//��ȡ��ǰǽ�ڵ�����
	bool wall_checkpos(int food_x, int food_y);//��������ɵ�ʳ���Ƿ��������ǽ����
	void wall_reset();//����ǽ������
};
