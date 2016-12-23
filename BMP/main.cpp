#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include "bmp_structure.h"  

using namespace std;

int main(){
	IP a;
	//加载图片
	a.loadPic(".\\pics\\lena.BMP");

	//图片信息
	a.showBmpFileHead();
	a.showBmpInfoHead();
	//a.showPixels();
	//a.colorInvert(25);
	//a.savePic("tmp.BMP");
	
	//色域转换
	//a.RGB2YIQ("yiq.BMP");
	//a.RGB2HSI("hsi.BMP");
	//a.RGB2XYZ("xyz.BMP");

	//二值化处理
	//a.invide(".\\pics\\invide.bmp", 180);
	//a.OSTU(".\\pics\\invide.bmp");

	//添加噪声
	a.addSalt(150, 150);
	a.savePic(".\\pics\\salt.bmp");
	
	system("pause");
}