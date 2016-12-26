#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include "bmp_structure.h"  

using namespace std;

int main(){
	IP a;

	/********************实验一********************/
	//加载图片到内存。
	a.loadPic(".\\pics\\lena_gray_dct.BMP");

	//图片信息，下面三个函数分别是显示文件头，显示信息头，显示位图数据
	//a.showBmpFileHead();
	//a.showBmpInfoHead();
	//a.showPixels();
	
	//色域转换
	//a.RGB2YIQ();
	//a.RGB2HSI();
	//a.RGB2XYZ();

	/********************实验二********************/
	//a.RGB2YCrCb();
	//a.DCT();
	//a.DFT();
	a.IDCT();
	//a.IDFT();


	/********************实验三********************/
	//二值化处理
	//a.invide(100);
	//a.OSTU();
	//a.halfTone();

	//添加噪声，分别是椒盐噪声，高斯噪声
	//a.addSaltPeppers(150, 150);
	//a.addGaussian();
	
	//转为灰度图
	//a.color2gray();

	//滤波，中值滤波，均值滤波
	//a.medianFilter();
	//a.meanFilter();

	/********************实验四********************/
	//模板操作，laplacian算子
	//a.convolution(vector<vector<int>>{{0,-1,0},{-1,4,-1},{0,-1,0}});

	//直方图均衡化,规定化
	//a.HistogramEqualization();
	//a.HistogramMatch(vector<double>(256, 1.0/256.0));
	//cout << a.SumGray(0, 0, 511, 511);

	//保存图片
	a.savePic(".\\pics\\lena_gray_idct.bmp");
	
	system("pause");
}