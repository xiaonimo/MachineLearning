#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include "bmp_structure.h"  

using namespace std;

int main(){
	IP a;
	//����ͼƬ
	a.loadPic(".\\pics\\lena.BMP");

	//ͼƬ��Ϣ
	a.showBmpFileHead();
	a.showBmpInfoHead();
	//a.showPixels();
	//a.colorInvert(25);
	//a.savePic("tmp.BMP");
	
	//ɫ��ת��
	//a.RGB2YIQ("yiq.BMP");
	//a.RGB2HSI("hsi.BMP");
	//a.RGB2XYZ("xyz.BMP");

	//��ֵ������
	//a.invide(".\\pics\\invide.bmp", 180);
	//a.OSTU(".\\pics\\invide.bmp");

	//�������
	a.addSalt(150, 150);
	a.savePic(".\\pics\\salt.bmp");
	
	system("pause");
}