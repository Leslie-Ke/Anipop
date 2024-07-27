#include <stdio.h>
#include <graphics.h>//easyx图形库的头文件

//开发日志
//1.构建初始的界面

#define WIN_WIDHT 485
#define	WIN_HEIGHT 917

IMAGE imgBg; //表示背景图片
void init() {
	//创建游戏窗口
	initgraph(WIN_WIDHT, WIN_HEIGHT);
	loadimage(&imgBg, _T("D:/消消乐-素材/res/bg2.png"));
	//_T("...") 是一个宏，用于在 Unicode 和 ASCII 编译设置之间自动切换。
}

void updateWindow() {
	putimage(0, 0, &imgBg);
}
int main() {
	init();//初始化
	updateWindow();//用来更新窗口
	system("pause");
	return 0;
}