#include <stdio.h>
#include <graphics.h>//easyx图形库的头文件
#include <time.h>
#include <tchar.h>
#include <math.h>
#include "tools.h"//用于实现取消数组图标的小黑边

//开发日志
//1.构建初始的界面
//2.构建初始的方块数组
//3.解决游戏界面中图标小黑边问题，完成游戏基础界面的全部设计
//4.设计游戏架构和方块移动（方块移动未实现）
//5.实现方块间的移动，但是不能判断能否消除方块

#define WIN_WIDHT			485
#define	WIN_HEIGHT			817
#define ROWS				8
#define COLS				8
#define BLOCK_TYPE_COUNT    7

IMAGE imgBg; //表示背景图片
IMAGE imgBlocks[BLOCK_TYPE_COUNT];

struct block{
	int type;//方块的类型，0表示空白
	int x, y;
	int row, col;//表示行和列
};

struct block map[ROWS+2][COLS+2];//给数组扩充这样就不用担心越界

const int off_x = 17;//定义数组在水平方向上距离
const int off_y = 174;//定义数组在竖直方向的距离
const int block_size = 52;//小方块尺寸

int click;//表示相邻两个位置的单击次数，第二次点击时才会进行交换
int posX1, posY1;//第一次单击的行和列
int posX2, posY2;//第二次单击的行和列

bool isMoving;//表示当前是否在移动
bool isSwap; //当单击两个相邻的方块后设置为true

void init() {
	//创建游戏窗口
	initgraph(WIN_WIDHT, WIN_HEIGHT, 1);
	loadimage(&imgBg, _T("res/bg2.png"),WIN_WIDHT,WIN_HEIGHT,true);
	//_T("...") 是一个宏，用于在 Unicode 和 ASCII 编译设置之间自动切换。

	char name[64];
	for (int i = 0; i < BLOCK_TYPE_COUNT; i++)
	{
		sprintf_s(name, sizeof(name), _T("res/%d.png"), i + 1);
		loadimage(&imgBlocks[i], name,block_size,block_size,true);

	}

	//配置随机数种子
	srand(time(NULL));

	//初始化方块数组
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
	isMoving = false;
	isSwap = false;
}

void updateWindow() {
	BeginBatchDraw();//开始双缓冲
	putimage(0, 0, &imgBg);

	for (int i = 1; i <= ROWS; i++) {
		for (int j = 1; j <= COLS; j++) {
			if (map[i][j].type ) {
				IMAGE* img = &imgBlocks[map[i][j].type - 1];
				putimagePNG(map[i][j].x, map[i][j].y, img);
			}
		}
	}
	EndBatchDraw();//结束双缓冲
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
	ExMessage msg;//最新版本的easyx版本自带
	if (peekmessage(&msg) && msg.message == WM_LBUTTONDOWN) {
		
		/*map[i][j].x = off_x + (j - 1) * (block_size + 5);
		map[i][j].y = off_y + (i - 1) * (block_size + 5);*/
		if (msg.x < off_x || msg.y < off_y) return;
		
		int col = (msg.x - off_x) / (block_size + 5) + 1;
		int row = (msg.y - off_y) / (block_size + 5) + 1;

		if (col > COLS || row > ROWS) return;
		//确保用户点击位置在数组内
		
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
				//后续音效
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
	//发生移动后，而且这个单向移动已经结束
	if (isSwap && !isMoving) {
		//如果没有匹配到三个或者三个以上的方块，就设置还原
		if (1) {//后续优化，先完成还原的基础逻辑代码
			exchange(posY1, posX1, posY2, posX2);
		}

		isSwap = false;
	}
}
int main() {
	init();//初始化
	
	while (true)
	{
		userClick();//处理用户点击操作
		move();//方块移动
		huanYuan();//还原
		updateWindow();//用来更新窗口

		Sleep(10);//帧等待(后续考虑优化)
	}

	system("pause");
	return 0;
}