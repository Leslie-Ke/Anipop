#include <stdio.h>
#include <graphics.h>//easyxͼ�ο��ͷ�ļ�
#include <time.h>
#include <tchar.h>
#include <math.h>
#include <mmsystem.h> //�������������ͷ�ļ�
#include "tools.h"//����ʵ��ȡ������ͼ���С�ڱ�
#pragma comment(lib,"winmm.lib")//�������ֵĿ��ļ�

//������־
//1.������ʼ�Ľ���
//2.������ʼ�ķ�������
//3.�����Ϸ������ͼ��С�ڱ����⣬�����Ϸ���������ȫ�����
//4.�����Ϸ�ܹ��ͷ����ƶ��������ƶ�δʵ�֣�
//5.ʵ�ַ������ƶ������ǲ����ж��ܷ���������
//6.����ʵ�ַ���Ļ�ԭ
//7.��ɷ��黹ԭ���ж�����
//8.ʵ�ַ�����������

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
	int match;//ƥ�����
	int tomin;//͸���ȣ�0-255.0��ȫ͸��
};

struct block map[ROWS+2][COLS+2];//���������������Ͳ��õ���Խ��

const int off_x = 17;//����������ˮƽ�����Ͼ���
const int off_y = 174;//������������ֱ����ľ���
const int block_size = 52;//С����ߴ�

int click;//��ʾ��������λ�õĵ����������ڶ��ε��ʱ�Ż���н���
int posX1, posY1;//��һ�ε������к���
int posX2, posY2;//�ڶ��ε������к���

bool isMoving;//��ʾ��ǰ�Ƿ����ƶ�
bool isSwap; //�������������ڵķ��������Ϊtrue

int score;//���÷���

void init() {
	//������Ϸ����
	initgraph(WIN_WIDHT, WIN_HEIGHT,1);
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
			map[i][j].type = 1 + rand()%7;
			map[i][j].row = i;
			map[i][j].col = j;
			map[i][j].x = off_x + (j - 1) * (block_size + 5);
			map[i][j].y = off_y + (i - 1) * (block_size + 5);
			map[i][j].match = 0;
			map[i][j].tomin = 255;
		}
	}

	click = 0;
	isMoving = false;
	isSwap = false;
	score = 0;
	setFont("Segoe UI Black", 20, 40);//���÷����������С�͸�ʽ

	mciSendString("play res/bg.mp3 repeat", 0, 0, 0);//���ű�������
	mciSendString("play res/start.mp3", 0, 0, 0);//��ʼ����
	//mciSendString("setaudio res/start.mp3 volume to 90", 0, 0, 0);//��������Ϊ40%
}

void updateWindow() {
	BeginBatchDraw();//��ʼ˫����
	putimage(0, 0, &imgBg);

	for (int i = 1; i <= ROWS; i++) {
		for (int j = 1; j <= COLS; j++) {
			if (map[i][j].type ) {
				IMAGE* img = &imgBlocks[map[i][j].type - 1];
				putimageTMD(map[i][j].x, map[i][j].y, img,map[i][j].tomin);
			}
		}
	}
	char scoreStr[16];
	sprintf_s(scoreStr, sizeof(scoreStr), " %d", score);
	int x = 394 + (75 - strlen(scoreStr) * 20) / 2;
	outtextxy(x, 60, scoreStr);

	EndBatchDraw();//����˫����
}

void exchange(int row1, int col1, int row2, int col2) {
	struct block tmp = map[row1][col1];
	map[row1][col1] = map[row2][col2];
	map[row2][col2]  = tmp;

	map[row1][col1].row = row1;
	map[row1][col1].col = col1;
	map[row2][col2].col = col2;
	map[row2][col2].row = row2;
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
				isSwap = true;
				//������Ч
				PlaySound("res/pao.wav", 0, SND_FILENAME | SND_ASYNC);//���ļ����첽����
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
	isMoving = false;
	for (int i = ROWS; i > 0; i--) {
		for (int j = 1; j <= COLS; j++) {
			struct block* p = &map[i][j];

			int dx, dy;
			for (int k = 0; k < 4; k++) {

				int x = off_x + (p->col - 1) * (block_size + 5);
				int y = off_y + (p->row - 1) * (block_size + 5);

				dx = p->x - x;
				dy = p->y - y;

				if (dx) p->x -= dx / abs(dx);
				if (dy) p->y -= dy / abs(dy);
			}

			if (dx || dy) isMoving = true;
		}
	}
}

void huanYuan() {
	//�����ƶ��󣬶�����������ƶ��Ѿ�����
	if (isSwap && !isMoving) {
		//���û��ƥ�䵽���������������ϵķ��飬�����û�ԭ

		int count = 0;
		for (int i = 1; i <= ROWS; i++) {
			for (int j = 1; j <= COLS; j++) {
				count += map[i][j].match;
			}
		}
		if (count == 0) {//�����Ż�������ɻ�ԭ�Ļ����߼�����
			exchange(posY1, posX1, posY2, posX2);
		}

		isSwap = false;
	}
}

void check() {
	for (int i = 1; i <= ROWS; i++) {
		for (int j = 1; j <= COLS; j++) {
			if (map[i][j].type == map[i - 1][j].type && map[i][j].type == map[i + 1][j].type) {
				for (int k = -1; k <= 1; k++) {
					map[i + k][j].match++;
				}
			}
			if (map[i][j].type == map[i][j - 1].type && map[i][j].type == map[i][j + 1].type) {
				for (int k = -1; k <= 1; k++) {
					map[i][j + k].match++;
				}
			}
		}
	}
}

void xiaochu() {
	bool flag = false;
	for (int i = 1; i <= ROWS; i++) {
		for (int j = 1; j <= COLS; j++) {
			if (map[i][j].match && map[i][j].tomin > 10) {
				if (map[i][j].tomin == 255) {
					flag = true;
				}
				map[i][j].tomin -= 10;
				isMoving = true;
			}
		}
	}

	if (flag) {
		PlaySound("res/clear.wav", 0, SND_FILENAME | SND_ASYNC);
	}
}

void  updateGame() {
	//�½�
	for (int i = ROWS; i >= 1; i--) {
		for (int j = 1; j <= COLS; j++) {
			if (map[i][j].match) {
				for (int k = i - 1; k >= 1; k--) {
					if (map[k][j].match == 0) {
						exchange(k, j, i, j);
						break;
					}
				}
			}
		}
	}
	//�����·�����н��䴦��
	for (int i = 1; i <= COLS; i++) {
		int n = 0;
		for (int j = ROWS; j >= 1; j--) {
			if (map[i][j].match) {
				map[i][j].type = 1 + rand() % 7;
				map[i][j].y = off_y - (n + 1) * (block_size + 5);
				n++;
				map[i][j].match = 0;
				map[i][j].tomin = 255;
			}
		}
		score += n;
	}
}

void checkScore() {
	if (score > 90) {
		setbkcolor(BLACK); // ���ñ�����ɫΪ��ɫ
		settextcolor(BLACK); // ����������ɫΪ��ɫt
		settextstyle(40, 20, _T("Segoe UI Black")); // ������������ʹ�С

		char ms[64];
		sprintf_s(ms, sizeof(ms), "��ս�ɹ�! ��ķ�����%d", score);
		/*char ms2[64];
		sprintf_s(ms2, sizeof(ms2), "60���������ս");*/
		char ms3[64];
		sprintf_s(ms3, sizeof(ms3), "�رտ���̨���ɹر���Ϸ");
		

		// ����Ļ������ʾ��ʾ
		int x = (WIN_WIDHT - textwidth(ms)) / 2;
		int y = (WIN_HEIGHT - textheight(ms)) / 2;
		outtextxy(x, y, ms);
		//outtextxy(x, y + 40, ms2);
		outtextxy(x, y + 80, ms3);

		// ��ͣһ��ʱ�������û�������ʾ
		Sleep(60000); // ��ʾ60��
		
		// ���³�ʼ����Ϸ״̬
		//init();
	}
}

int main() {
	init();//��ʼ��
	
	while (true)
	{
		if (!isMoving) { userClick(); }//�����û��������
		check();//ƥ��������
		move();//�����ƶ�
		if (!isMoving) { xiaochu(); }//������������
		huanYuan();//��ԭ
		updateWindow();//�������´���
		if (!isMoving) { updateGame();}//������Ϸ���ݣ�ʵ�ַ��齵�书���Լ������������ʾ
		if (score >= 90) { checkScore(); }
		if(isMoving)Sleep(10);//֡�ȴ�(���������Ż�)
	}

	system("pause");
	return 0;
}