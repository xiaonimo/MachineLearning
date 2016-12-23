#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "bmp_structure.h"

using namespace std;

void IP::loadPic(const char* filename) {  
	//打开文件
	FILE* fpi = fopen(filename, "rb");
	if (!fpi) {
		cout << "Failed to open the picture!" << endl;
		return;
	}

	//读取文件类型
	fread(&_type, 1, sizeof(WORD), fpi);
	if (0x4d42 != _type) {
		cout << "The picture is not BMP!" << endl;
		return;
	}

	//读取文件头
	_file_head_size = fread(&file_header, 1, sizeof(tagBITMAPFILEHEADER), fpi);

	//读取信息头
	_info_head_size = fread(&info_header, 1, sizeof(tagBITMAPINFOHEADER), fpi);  

	//读取调色板  
	_quad_size = 0;
	for (unsigned int i = 0; i < info_header.biClrUsed; ++i) {
		tagRGBQUAD q;
		_quad_size += fread(&q, 1, sizeof(tagRGBQUAD), fpi);
		quad.push_back(q);
	}

	//读取像素值
	int width = info_header.biWidth;
	int height = info_header.biHeight;
	data = vector<vector<tagIMAGEDATA>>(height, vector<tagIMAGEDATA>(width));
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			fread(&data[i][j], 1, sizeof(tagIMAGEDATA), fpi);
		}
	}
	data_tmp = data;
	fclose(fpi);
}

void IP::savePic(const char* filename) {
	//创建文件
	FILE* fpw = fopen(filename, "wb");
	if (!fpw)
	{
		cout << "Failed to create the bmp file" << endl;
		return;
	}
	//写入文件类型
	WORD bfType_w = 0x4d42;
	fwrite(&bfType_w, 1, sizeof(WORD), fpw);  

	//写入文件头
	fwrite(&file_header, 1, sizeof(tagBITMAPFILEHEADER), fpw);
	
	//写入信息头
	fwrite(&info_header, 1, sizeof(tagBITMAPINFOHEADER), fpw);
	
	//保存调色板数据  
	for (auto q:quad) {
		fwrite(&q, 1, sizeof(tagRGBQUAD), fpw);
	}

	//保存像素数据
	int width = info_header.biWidth;
	int height = info_header.biHeight;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			fwrite(&data_tmp[i][j], 1, sizeof(tagIMAGEDATA), fpw);
		}
	}
	fclose(fpw);
	
}

void IP::showBmpFileHead(){
	cout << "---File Header("	<< _file_head_size << ")---:" << endl;
	cout << "bfType:\t\t"		<< hex << _type << dec << endl;
	cout << "bfSize:\t\t"		<<file_header.bfSize << endl;
	cout << "bfReserved1:\t"	<<file_header.bfReserved1 << endl;
	cout << "bfReserved2:\t"	<<file_header.bfReserved2 << endl;
	cout << "bfOffBits:\t"		<<file_header.bfOffBits << endl << endl;
}

void IP::showBmpInfoHead(){
	cout << "---Info Header("		<<_info_head_size << ")---" << endl;
	cout << "biSize:\t\t\t"			<<info_header.biSize << endl;
	cout << "biWidth:\t\t"			<<info_header.biWidth << endl;
	cout << "biHeight:\t\t"			<<info_header.biHeight << endl;
	cout << "biPlanes:\t\t"			<<info_header.biPlanes << endl;
	cout << "biBitCount:\t\t"		<<info_header.biBitCount << endl;
	cout << "biCompression:\t\t"	<<info_header.biCompression << endl;
	cout << "biSizeImage:\t\t"		<<info_header.biSizeImage << endl;
	cout << "biXPelsPerMeter:\t"	<<info_header.biXPelsPerMeter << endl;
	cout << "biYPelsPerMeter:\t"	<<info_header.biYPelsPerMeter << endl;
	cout << "biClrUsed:\t\t"		<<info_header.biClrUsed << endl;
	cout << "biClrImportant:\t\t"	<<info_header.biClrImportant << endl;
	cout << "quad_size:\t\t"		<< _quad_size << endl;
}

void IP::showPixels() {
	if (data.empty()) {
		cout << "Error in picture!" << endl;
		return;
	}
	cout << "(" << data.size() << "," << data[0].size() << "," << 
		sizeof(data[0][0])/sizeof(BYTE) <<")" << endl;
	for (auto v : data) {
		for (auto p : v)
			//cout << (int)p.blue << " ";
			printf("(%d,%d,%d) ", p.blue, p.green, p.red);
		cout << endl;
	}
}

tagIMAGEDATA IP::getPixels(long int x, long int y) {
	if (data.empty()) {
		cout << "Error in picture!" << endl;
		return tagIMAGEDATA();
	}
	if (x <0 || y < 0 ||
		x >= info_header.biHeight || y >= info_header.biWidth) {
		cout << "Error Coordinate" << endl;
		return tagIMAGEDATA();
	}
	return data[x][y];
}

void IP::colorInvert(unsigned char maxPixel) {
	for (auto &v : data_tmp) {
		for (auto &p : v) {
			p.blue  += maxPixel;
			p.green += maxPixel;
			p.red   += maxPixel;
		}
	}
}

void IP::RGB2YIQ(const char* filename) {
	for (int i = 0; i < info_header.biHeight;++i) {
		for (int j = 0; j < info_header.biWidth;++j) {
			BYTE b = data[i][j].blue, g = data[i][j].green, r = data[i][j].red;
			data_tmp[i][j].blue  = BYTE(0.211*r - 0.523*g + 0.312*b);
			data_tmp[i][j].green = BYTE(0.596*r - 0.274*g - 0.322*b);
			data_tmp[i][j].red   = BYTE(0.299*r + 0.587*g + 0.144*b);
		}
	}
	savePic(filename);
}

void IP::RGB2HSI(const char* filename) {
	for (int i = 0; i < info_header.biHeight; ++i) {
		for (int j = 0; j < info_header.biWidth; ++j) {
			BYTE b = data[i][j].blue, g = data[i][j].green, r = data[i][j].red;
			data_tmp[i][j].blue = BYTE((r + g + b) / 3.0);
			data_tmp[i][j].green = BYTE(1 - (3.0 / (r + g + b))*min(r, min(g, b)));
			data_tmp[i][j].red = BYTE(acos((2 * r - g - b) / 2.0) / (pow(r - g, 2) + (r - b)*sqrt(g - b)));
		}
	}
	savePic(filename);
}

void IP::RGB2XYZ(const char* filename) {
	for (int i = 0; i < info_header.biHeight; ++i) {
		for (int j = 0; j < info_header.biWidth; ++j) {
			BYTE b = data[i][j].blue, g = data[i][j].green, r = data[i][j].red;
			data_tmp[i][j].blue = BYTE(0.000*r - 0.01*g + 0.99*b);
			data_tmp[i][j].green = BYTE(0.177*r - 0.813*g - 0.011*b);
			data_tmp[i][j].red = BYTE(0.49*r + 0.31*g + 0.2*b);
		}
	}
	savePic(filename);
}

void IP::invide(const char* filename, BYTE val) {
	for (int i = 0; i < info_header.biHeight; ++i) {
		for (int j = 0; j < info_header.biWidth; ++j) {
			BYTE bi = data[i][j].blue > val ? 255 : 0;
			data_tmp[i][j].blue = bi;
			data_tmp[i][j].green = bi;
			data_tmp[i][j].red = bi;
		}
	}
	savePic(filename);
}

void IP::OSTU(const char* filename) {
	BYTE res = 0;
	//计算直方图
	unsigned int gray_cnt[256] = {0};
	double gray_pro[256] = { 0. };
	for (auto v : data) {
		for (auto p : v)
			gray_cnt[p.blue]++;
	}

	//计算总体像素
	unsigned int sum_gray_val = 0;
	unsigned int cnt = 0; 
	for (int i = 0; i < 256; ++i) {
		sum_gray_val += i*gray_cnt[i];
		cnt += gray_cnt[i];
	}

	//计算概率
	for (int i = 0; i < 256; ++i) {
		gray_pro[i] = gray_cnt[i] / (double)cnt;
	}

	//计算最佳灰度值
	double max_varience = 0.;
	double cur_varience = 0.;
	double av_gl = 0., av_gr = 0.;
	double av_gg = sum_gray_val / (double)cnt;
	double w0 = 0., w1 = 0.;
	unsigned int cur_gray_val = 0;
	unsigned int nl = 0, nr = 0;
	for (int i = 0; i < 256; ++i) {
		nl += gray_cnt[i];
		nr = cnt - nl;
		if (nl == 0) continue;
		if (nr == 0) break;
		w0 += gray_pro[i];
		w1 = 1 - w0;
		cur_gray_val += i*gray_cnt[i];
		av_gl = cur_gray_val / double(nl);
		av_gr = (sum_gray_val - cur_gray_val) / double(nr);
		cur_varience = w0*pow(av_gl-av_gg, 2) + w1*pow(av_gr-av_gg,2);
		if (cur_varience > max_varience) {
			res = BYTE(i);
			max_varience = cur_varience;
		}
	}
	//printf("%d", res);
	invide(filename, res);
}

void IP::addSalt(unsigned int salt, unsigned int pepper) {
	srand(time(nullptr));
	unsigned int w = info_header.biWidth;
	unsigned int h = info_header.biHeight;
	//add salt
	for (unsigned int i = 0; i < salt; ++i) {
		unsigned int col = (unsigned)rand() % w;
		unsigned int row = (unsigned)rand() % h;
		data_tmp[col][row].blue = 0;
		data_tmp[col][row].green = 0;
		data_tmp[col][row].red = 0;
	}

	//add pepper
	for (unsigned int i = 0; i < pepper; ++i) {
		unsigned int col = rand() % w;
		unsigned int row = rand() % h;
		data_tmp[col][row].blue = 255;
		data_tmp[col][row].green = 255;
		data_tmp[col][row].red = 255;
	}
}

void IP::addGaussian(unsigned int g) {
	srand(time(nullptr));
	unsigned int w = info_header.biWidth;
	unsigned int h = info_header.biHeight;
	//set 0
	for (auto &v : data_tmp) {
		for (auto &p : v) {
			p.blue = 0;
			p.green = 0;
			p.red = 0;
		}
	}
	
	//多次添加噪声
	for (int i = 0; i < h; ++i) {
		
	}
}