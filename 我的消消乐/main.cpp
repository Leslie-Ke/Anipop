#include <stdio.h>
#include <graphics.h>//easyxͼ�ο��ͷ�ļ�
#include <time.h>
#include <tchar.h>
//������־
//1.������ʼ�Ľ���
//2.������ʼ�ķ�������

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

const int off_x = 17;
const int off_y = 274;
const int block_size = 52;

void init() {
	//������Ϸ����
	initgraph(WIN_WIDHT, WIN_HEIGHT);
	loadimage(&imgBg, _T("D:/������-�ز�/res/bg2.png"),WIN_WIDHT,WIN_HEIGHT,true);
	//_T("...") ��һ���꣬������ Unicode �� ASCII ��������֮���Զ��л���

	char name[64];
	for (int i = 0; i < BLOCK_TYPE_COUNT; i++)
	{
		sprintf_s(name, sizeof(name), _T("D:/������-�ز�/res/%d.png"), i + 1);
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
}

void updateWindow() {
	putimage(0, 0, &imgBg);

	for (int i = 1; i < ROWS; i++) {
		for (int j = 1; j < COLS; j++) {
			if (map[i][j].type ) {
				IMAGE* img = &imgBlocks[map[i][j].type - 1];
				putimage(map[i][j].x, map[i][j].y, img);
			}
		}
	}
}
int main() {
	init();//��ʼ��
	updateWindow();//�������´���
	system("pause");
	return 0;
}