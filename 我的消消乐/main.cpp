#include <stdio.h>
#include <graphics.h>//easyxͼ�ο��ͷ�ļ�

//������־
//1.������ʼ�Ľ���

#define WIN_WIDHT 485
#define	WIN_HEIGHT 917

IMAGE imgBg; //��ʾ����ͼƬ
void init() {
	//������Ϸ����
	initgraph(WIN_WIDHT, WIN_HEIGHT);
	loadimage(&imgBg, _T("D:/������-�ز�/res/bg2.png"));
	//_T("...") ��һ���꣬������ Unicode �� ASCII ��������֮���Զ��л���
}

void updateWindow() {
	putimage(0, 0, &imgBg);
}
int main() {
	init();//��ʼ��
	updateWindow();//�������´���
	system("pause");
	return 0;
}