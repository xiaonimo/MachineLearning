#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include "bmp_structure.h"  

using namespace std;

int main(){
	IP a;

	/********************ʵ��һ********************/
	//����ͼƬ���ڴ档
	a.loadPic(".\\pics\\lena_gray_dct.BMP");

	//ͼƬ��Ϣ���������������ֱ�����ʾ�ļ�ͷ����ʾ��Ϣͷ����ʾλͼ����
	//a.showBmpFileHead();
	//a.showBmpInfoHead();
	//a.showPixels();
	
	//ɫ��ת��
	//a.RGB2YIQ();
	//a.RGB2HSI();
	//a.RGB2XYZ();

	/********************ʵ���********************/
	//a.RGB2YCrCb();
	//a.DCT();
	//a.DFT();
	a.IDCT();
	//a.IDFT();


	/********************ʵ����********************/
	//��ֵ������
	//a.invide(100);
	//a.OSTU();
	//a.halfTone();

	//����������ֱ��ǽ�����������˹����
	//a.addSaltPeppers(150, 150);
	//a.addGaussian();
	
	//תΪ�Ҷ�ͼ
	//a.color2gray();

	//�˲�����ֵ�˲�����ֵ�˲�
	//a.medianFilter();
	//a.meanFilter();

	/********************ʵ����********************/
	//ģ�������laplacian����
	//a.convolution(vector<vector<int>>{{0,-1,0},{-1,4,-1},{0,-1,0}});

	//ֱ��ͼ���⻯,�涨��
	//a.HistogramEqualization();
	//a.HistogramMatch(vector<double>(256, 1.0/256.0));
	//cout << a.SumGray(0, 0, 511, 511);

	//����ͼƬ
	a.savePic(".\\pics\\lena_gray_idct.bmp");
	
	system("pause");
}