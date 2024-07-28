#include <stdio.h>
#include <graphics.h>//easyxͼ�ο��ͷ�ļ�
#include <time.h>
#include <tchar.h>
#include <math.h>
#include "tools.h"//����ʵ��ȡ������ͼ���С�ڱ�

//������־
//1.������ʼ�Ľ���
//2.������ʼ�ķ�������
//3.�����Ϸ������ͼ��С�ڱ����⣬�����Ϸ���������ȫ�����
//4.�����Ϸ�ܹ��ͷ����ƶ��������ƶ�δʵ�֣�

#define WIN_WIDHT			485
#define	WIN_HEIGHT			817
#define ROWS				8
#define COLS				8
#define BLOCK_TYPE_COUNT    7

IMAGE imgBg; //��ʾ����ͼƬ
IMAGE imgBlocks[BLOCK_TYPE_COUNT];

struct block{
	int type;//��������ͣ�0��ʾ�հ�
	int x, y;
	int row, col;//��ʾ�к���
};

struct block map[ROWS+2][COLS+2];//���������������Ͳ��õ���Խ��

const int off_x = 17;//����������ˮƽ�����Ͼ���
const int off_y = 174;//������������ֱ����ľ���
const int block_size = 52;//С����ߴ�

int click;//��ʾ��������λ�õĵ����������ڶ��ε��ʱ�Ż���н���
int posX1, posY1;//��һ�ε������к���
int posX2, posY2;//�ڶ��ε������к���

void init() {
	//������Ϸ����
	initgraph(WIN_WIDHT, WIN_HEIGHT, 1);
	loadimage(&imgBg, _T("res/bg2.png"),WIN_WIDHT,WIN_HEIGHT,true);
	//_T("...") ��һ���꣬������ Unicode �� ASCII ��������֮���Զ��л���

	char name[64];
	for (int i = 0; i < BLOCK_TYPE_COUNT; i++)
	{
		sprintf_s(name, sizeof(name), _T("res/%d.png"), i + 1);
		loadimage(&imgBlocks[i], name,block_size,block_size,true);

	}

	//�������������
	srand(time(NULL));

	//��ʼ����������
	for ( int i = 1; i <= ROWS; i++)
	{
		for ( int j = 1; j <= COLS; j++) {
			map[i][j].type = 1 + rand()%4;
			map[i][j].row = i;
			map[i][j].col = j;
			map[i][j].x = off_x + (j - 1) * (block_size + 5);
			map[i][j].y = off_y + (i - 1) * (block_size + 5);
		}
	}

	click = 0;
}

void updateWindow() {
	BeginBatchDraw();//��ʼ˫����
	putimage(0, 0, &imgBg);

	for (int i = 1; i <= ROWS; i++) {
		for (int j = 1; j <= COLS; j++) {
			if (map[i][j].type ) {
				IMAGE* img = &imgBlocks[map[i][j].type - 1];
				putimagePNG(map[i][j].x, map[i][j].y, img);
			}
		}
	}
	EndBatchDraw();//����˫����
}

void exchange(int row1, int col1, int row2, int col2) {
	// to do.
}

void userClick() {
	ExMessage msg;//���°汾��easyx�汾�Դ�
	if (peekmessage(&msg) && msg.message == WM_LBUTTONDOWN) {
		
		/*map[i][j].x = off_x + (j - 1) * (block_size + 5);
		map[i][j].y = off_y + (i - 1) * (block_size + 5);*/
		if (msg.x < off_x || msg.y < off_y) return;
		
		int col = (msg.x - off_x) / (block_size + 5) + 1;
		int row = (msg.y - off_y) / (block_size + 5) + 1;

		if (col > COLS || row > ROWS) return;
		//ȷ���û����λ����������
		
		click++;
		if (click == 1) {
			posX1 = col;
			posY1 = row;
		}
		else if (click == 2) {
			posX2 = col;
			posY2 = row;

			if (abs(posX2 - posX1) + abs(posY2 - posY1) == 1) {
				exchange(posY1, posX1, posY2, posX2);
				click = 0;
				//������Ч
			}
			else {
				click = 1;
				posX1 = col;
				posY1 = row;
			}
		}
	}

}

void move() {

}

void huanYuan() {

}
int main() {
	init();//��ʼ��
	
	while (true)
	{
		userClick();//�����û��������
		move();//�����ƶ�
		huanYuan();//��ԭ
		updateWindow();//�������´���

		Sleep(10);//֡�ȴ�(���������Ż�)
	}

	system("pause");
	return 0;
}