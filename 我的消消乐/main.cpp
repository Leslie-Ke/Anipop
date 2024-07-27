#include <stdio.h>
#include <graphics.h>//easyx图形库的头文件
#include <time.h>
#include <tchar.h>
//开发日志
//1.构建初始的界面
//2.构建初始的方块数组

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

const int off_x = 17;
const int off_y = 274;
const int block_size = 52;

void init() {
	//创建游戏窗口
	initgraph(WIN_WIDHT, WIN_HEIGHT);
	loadimage(&imgBg, _T("D:/消消乐-素材/res/bg2.png"),WIN_WIDHT,WIN_HEIGHT,true);
	//_T("...") 是一个宏，用于在 Unicode 和 ASCII 编译设置之间自动切换。

	char name[64];
	for (int i = 0; i < BLOCK_TYPE_COUNT; i++)
	{
		sprintf_s(name, sizeof(name), _T("D:/消消乐-素材/res/%d.png"), i + 1);
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
	init();//初始化
	updateWindow();//用来更新窗口
	system("pause");
	return 0;
}