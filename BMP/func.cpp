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
	cout << "(" << data_tmp.size() << "," << data_tmp[0].size() << "," <<
		sizeof(data_tmp[0][0]) / sizeof(BYTE) << ")" << endl;
	for (auto v : data_tmp) {
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

void IP::color2gray() {
	for (auto &v : data_tmp) {
		for (auto &p : v) {
			BYTE av = BYTE((p.blue + p.green + p.red) / 3.0);
			p.blue  = av;
			p.green = av;
			p.red   = av;
		}
	}
}

void IP::RGB2YIQ() {
	for (int i = 0; i < info_header.biHeight;++i) {
		for (int j = 0; j < info_header.biWidth;++j) {
			BYTE b = data[i][j].blue, g = data[i][j].green, r = data[i][j].red;
			data_tmp[i][j].blue = BYTE(0.211*(double)r - 0.523*(double)g + 0.312*(double)b);
			data_tmp[i][j].green = BYTE(0.596*(double)r - 0.274*(double)g - 0.322*(double)b);
			data_tmp[i][j].red = BYTE(0.299*(double)r + 0.587*(double)g + 0.144*(double)b);
		}
	}
}

void IP::RGB2HSI() {
	for (int i = 0; i < info_header.biHeight; ++i) {
		for (int j = 0; j < info_header.biWidth; ++j) {
			double b = (double)data[i][j].blue, g = (double)data[i][j].green, r = (double)data[i][j].red;
			data_tmp[i][j].blue = BYTE((r + g + b) / 3.0);
			data_tmp[i][j].green = BYTE(1 - (3.0 / (r + g + b))*min(r, min(g, b)));
			data_tmp[i][j].red = BYTE(acos((2 * r - g - b) / 2.0) / (pow(r - g, 2) + (r - b)*sqrt(g - b)));
		}
	}
}

void IP::RGB2XYZ() {
	for (int i = 0; i < info_header.biHeight; ++i) {
		for (int j = 0; j < info_header.biWidth; ++j) {
			BYTE b = data[i][j].blue, g = data[i][j].green, r = data[i][j].red;
			data_tmp[i][j].blue = BYTE(0.000*(double)r - 0.01*(double)g + 0.99*(double)b);
			data_tmp[i][j].green = BYTE(0.177*(double)r - 0.813*(double)g - 0.011*(double)b);
			data_tmp[i][j].red = BYTE(0.49*(double)r + 0.31*(double)g + 0.2*(double)b);
		}
	}
}

void IP::RGB2YCrCb() {
	for (int i = 0; i < info_header.biHeight; ++i) {
		for (int j = 0; j < info_header.biWidth; ++j) {
			BYTE b = data[i][j].blue, g = data[i][j].green, r = data[i][j].red;
			//Y分量是第一个
			data_tmp[i][j].red = BYTE(-0.1687*(double)r - 0.3313*(double)g + 0.5*(double)b);
			data_tmp[i][j].green = BYTE(0.5*(double)r - 0.4187*(double)g - 0.0813*(double)b);
			data_tmp[i][j].blue = BYTE(0.299*(double)r + 0.587*(double)g + 0.114*(double)b);
		}
	}

}

void IP::invide(BYTE val) {
	for (int i = 0; i < info_header.biHeight; ++i) {
		for (int j = 0; j < info_header.biWidth; ++j) {
			BYTE bi = data[i][j].blue > val ? 255 : 0;
			data_tmp[i][j].blue = bi;
			data_tmp[i][j].green = bi;
			data_tmp[i][j].red = bi;
		}
	}
}

void IP::OSTU() {
	BYTE res = _get_threhold();
	//printf("%d", res);
	invide(res);
}

void IP::halfTone(BYTE threhold) {
	if (threhold == 0) 
		threhold = _get_threhold();
	vector<vector<int>> pos{ { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 } };
	vector<double> ratio{ 7 / 16.0, 1 / 16.0, 5 / 16.0, 3 / 16.0 };

	unsigned int w = info_header.biWidth;
	unsigned int h = info_header.biHeight;
	for (unsigned int i = 1; i < h - 1; ++i) {
		for (unsigned int j = 1; j < w - 1; ++j) {
			if (data_tmp[i][j].blue < threhold) {
				data_tmp[i][j].blue = 0;
				data_tmp[i][j].green = 0;
				data_tmp[i][j].red = 0;
				for (unsigned int k = 0; k < 4; ++k) {
					unsigned int x = i + pos[k][0];
					unsigned int y = j + pos[k][1];
					double val = double(threhold)*ratio[k] + double(data_tmp[x][y].blue);
					if (val > 255) {
						val = 255;
					}
					data_tmp[x][y].blue = (BYTE)val;
					data_tmp[x][y].green = (BYTE)val;
					data_tmp[x][y].red = (BYTE)val;
				}
			}
			else {
				data_tmp[i][j].blue = 255;
				data_tmp[i][j].green = 255;
				data_tmp[i][j].red = 255;
				for (unsigned int k = 0; k < 4; ++k) {
					unsigned int x = i + pos[k][0];
					unsigned int y = j + pos[k][1];
					double val = double(data_tmp[x][y].blue) - double(255 - threhold)*ratio[k];
					if (val < 0) {
						val =0;
					}
					data_tmp[x][y].blue = (BYTE)val;
					data_tmp[x][y].green = (BYTE)val;
					data_tmp[x][y].red = (BYTE)val;
				}
			}
		}
	}
}

void IP::addSaltPeppers(unsigned int salt, unsigned int pepper) {
	srand((unsigned)time(nullptr));
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

void IP::addGaussian() {
	srand((unsigned)time(nullptr));
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
	
	//多次叠加随机噪声，形成高斯噪声
	for (int i = 0; i < 10; ++i) {
		for (auto &v : data_tmp) {
			for (auto &p : v) {
				BYTE gau = BYTE(rand() % 10);
				p.blue += gau;
				p.green += gau;
				p.red += gau;
			}
		}
	}
	
	//与原图像叠加
	for (unsigned int i = 0; i < h; ++i) {
		for (unsigned int j = 0; j < w; ++j) {
			data_tmp[i][j].blue = (255 - data[i][j].blue>data_tmp[i][j].blue) ? (data[i][j].blue + data_tmp[i][j].blue) : 255;
			data_tmp[i][j].green = (255 - data[i][j].green>data_tmp[i][j].green) ? (data[i][j].green + data_tmp[i][j].green) : 255;
			data_tmp[i][j].red = (255 - data[i][j].red>data_tmp[i][j].red) ? (data[i][j].red + data_tmp[i][j].red) : 255;
		}
	}
}

void IP::medianFilter() {
	vector<vector<int>> pos{ { -1, -1 }, { -1, 0 }, { -1, 1 }, 
							 { 0, -1 },  {  0, 0 }, {  0, 1 }, 
							 { 1, -1 },  {  1, 0 }, {  1, 1 } };
	unsigned int w = info_header.biWidth;
	unsigned int h = info_header.biHeight;
	for (unsigned int i = 1; i < h - 1; ++i) {
		for (unsigned int j = 1; j < w - 1; ++j) {
			vector<tagIMAGEDATA> d(9);
			for (unsigned int k = 0; k < 9;++k) {
				d[k] = data[i + pos[k][0]][j + pos[k][1]];
			}
			vector<BYTE> m = _get_median(d);
			data_tmp[i][j].blue = m[0];
			data_tmp[i][j].green = m[1];
			data_tmp[i][j].red = m[2];
		}
	}
}

void IP::convolution(vector<vector<int>> kernel) {
	if (kernel.empty() || kernel.size() != 3 || kernel[0].size() != 3) {
		cout << "Error in kernel!" << endl;
		return;
	}
	unsigned w = info_header.biWidth;
	unsigned h = info_header.biHeight;
	vector<vector<int>> pos{ {-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,0}, {0,1}, {1,-1}, {1,0}, {1,1} };

	for (unsigned i = 1; i < h - 1; ++i) {
		for (unsigned j = 1; j < w - 1; ++j) {
			double b = 0., g = 0., r = 0.;
			for (unsigned m = 0; m < 3; ++m) {
				for (unsigned n = 0; n < 3; ++n) {
					b += double(kernel[m][n]) * double(data[i + pos[m * 3 + n][0]][j + pos[m * 3 + n][1]].blue);
					g += double(kernel[m][n]) * double(data[i + pos[m * 3 + n][0]][j + pos[m * 3 + n][1]].green);
					r += double(kernel[m][n]) * double(data[i + pos[m * 3 + n][0]][j + pos[m * 3 + n][1]].red);
				}
			}
			data_tmp[i][j].blue = BYTE(b<0 ? 0 : (b>255 ? 255 : b));
			data_tmp[i][j].green = BYTE(g<0 ? 0 : (g>255 ? 255 : g));
			data_tmp[i][j].red = BYTE(r<0 ? 0 : (r>255 ? 255 : r));
		}
	}
}

void IP::meanFilter() {
	vector<vector<int>> pos{ { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 } };
	unsigned int w = info_header.biWidth;
	unsigned int h = info_header.biHeight;
	for (unsigned int i = 1; i < h - 1; ++i) {
		for (unsigned int j = 1; j < w - 1; ++j) {
			double b = 0, g = 0, r = 0;
			for (unsigned int k = 0; k < 8; ++k) {
				b += double(data[i + pos[k][0]][j + pos[k][1]].blue);
				g += double(data[i + pos[k][0]][j + pos[k][1]].green);
				r += double(data[i + pos[k][0]][j + pos[k][1]].red);
			}
			data_tmp[i][j].blue =	BYTE(b / 8);
			data_tmp[i][j].green =	BYTE(g / 8);
			data_tmp[i][j].red =	BYTE(r / 8);
		}
	}
}

void IP::HistogramEqualization() {
	int gray_val[256] = { 0 };
	double gray_val_pro[256] = { 0. };
	double pCDF[256] = { 0. };
	BYTE CDF[256] = { 0 };
	double gmax = 0, gmin = 255;
	//统计灰度值
	for (auto v : data) {
		for (auto p : v) {
			if (p.blue < gmin) gmin = (double)p.blue;
			if (p.blue > gmax) gmax = (double)p.blue;
			gray_val[p.blue]++;
		}
	}

	//统计灰度分布
	double sum = double(info_header.biWidth*info_header.biHeight);
	for (unsigned int i = 0; i < 256; ++i)
		gray_val_pro[i] = gray_val[i] / sum;

	//计算CDF
	pCDF[0] = gray_val_pro[0];
	CDF[0] = BYTE(pCDF[0] * (gmax-gmin) + gmin + 0.5);
	for (unsigned int i = 1; i < 256; ++i) {
		pCDF[i] = pCDF[i - 1] + gray_val_pro[i];
		CDF[i] = BYTE(pCDF[i] * (gmax - gmin) + gmin + 0.5);
	}
	//cout << gmin << " " << gmax << endl;
	//映射
	unsigned w = info_header.biWidth;
	unsigned h = info_header.biHeight;
	for (unsigned i = 0; i < h; ++i) {
		for (unsigned j = 0; j < w; ++j) {
			data_tmp[i][j].blue = CDF[data_tmp[i][j].blue];
			data_tmp[i][j].green = data_tmp[i][j].blue;
			data_tmp[i][j].red = data_tmp[i][j].blue;
		
		}
	}
}

void IP::HistogramMatch(vector<double> spe_pro) {
	int gray_val[256] = { 0 };
	double gray_val_pro[256] = { 0. };
	double prCDF[256] = { 0. }, pzCDF[256] = {0.};
	BYTE rCDF[256] = { 0 };
	BYTE zCDF[256] = { 0 };
	//原图的最大最小灰度级
	double rmax = 0, rmin = 255;
	//规定图的最大最小灰度级
	double zmax = 0, zmin = 255;

	//统计原图灰度值
	for (auto v : data) {
		for (auto p : v) 
			gray_val[p.blue]++;
	}
	//统计原图灰度分布
	double sum = double(info_header.biWidth*info_header.biHeight);
	for (unsigned int i = 0; i < 256; ++i)
		gray_val_pro[i] = gray_val[i] / sum;

	//分别统计最大最小灰度
	for (unsigned i = 0; i < 256; ++i) {
		if (spe_pro[i] != 0) {
			zmax = i;
			if (i < zmin) zmin = i;
		}
		if (spe_pro[i] != 0) {
			zmax = i;
			if (i < zmin) zmin = i;
		}
	}

	//计算prCDF,pzCDF
	prCDF[0] = gray_val_pro[0];
	pzCDF[0] = spe_pro[0];
	rCDF[0] = BYTE(prCDF[0] * (rmax - rmin) + rmin + 0.5);
	zCDF[0] = BYTE(pzCDF[0] * (zmax - zmin) + zmin + 0.5);
	for (unsigned int i = 1; i < 256; ++i) {
		prCDF[i] = prCDF[i - 1] + gray_val_pro[i];
		rCDF[i] = BYTE(prCDF[i] * (rmax - rmin) + rmin + 0.5);
		pzCDF[i] = pzCDF[i - 1] + spe_pro[i];
		zCDF[i] = BYTE(pzCDF[i] * (zmax - zmin) + zmin + 0.5);
	}

	BYTE gray_min[256][256] = { 0 };
	for (unsigned i = 0; i < 256; ++i) {
		for (unsigned j = 0; j < 256; ++j) 
			gray_min[i][j] = abs(rCDF[i] - zCDF[j]);
	}

	//SML映射
	BYTE mapping[256] = { 0 };
	for (unsigned i = 0; i < 256; ++i) {
		unsigned  minx = 0;
		BYTE minVal = 255;;
		for (unsigned j = 0; j < 256; ++j)
		if (gray_min[j][i] < minVal) {
			minx = j;
			minVal = gray_min[j][i];
		}
		mapping[i] = minx;
	}
	for (auto &v : data_tmp) {
		for (auto &p : v) {
			BYTE c = mapping[unsigned(p.blue)];
			p.blue = c;
			p.green = c;
			p.red = c;
		}	
	}
}

int IP::SumGray(unsigned x1, unsigned y1, unsigned x2, unsigned y2) {
	unsigned w = info_header.biWidth;
	unsigned h = info_header.biHeight;
	if (x1 >= h || x2 >= h || y1 >= w || y2 >= w) {
		cout << "Wrong Coordinate!" << endl;
		return 0;
	}
	vector<vector<int>> sum(h, vector<int>(w));
	sum[0][0] = data[0][0].blue;
	for (unsigned i = 1; i < w; ++i)
		sum[0][i] = sum[0][i - 1] + (int)data[0][i].blue;
	for (unsigned i = 1; i < h; ++i)
		sum[i][0] = sum[i-1][0] + (int)data[i][0].blue;
	for (unsigned i = 1; i < h; ++i) {
		for (unsigned j = 1; j < w; ++j) {
			sum[i][j] = sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1] + (int)data[i][j].blue;
		}
	}
	return sum[x2][y2] - sum[x1][y1];
}

void IP::DCT() {
	unsigned w = info_header.biWidth;
	unsigned h = info_header.biHeight;
	_dctvec = vector<vector<double>>(h, vector<double>(w, 0.));
	for (unsigned i = 0; i < h; i += DCT_SIZE) {
		for (unsigned j = 0; j < w; j += DCT_SIZE)
			_DCT(i, j);
	}
	cout << "DCT matrix" << endl;
	for (unsigned i = 0; i < DCT_SIZE; ++i) {
		for (unsigned j = 0; j < DCT_SIZE; ++j)
			printf("%4.1f\t", _dctvec[i][j]);
		cout << endl;
	}
}

void IP::IDCT(unsigned cnt) {
	/*_mask = { { 1, 1, 1, 1, 1, 1, 1, 1 },
			  { 1, 1, 1, 1, 1, 1, 1, 0 },
			  { 1, 1, 1, 1, 1, 1, 0, 0 },
			  { 1, 1, 1, 1, 1, 0, 0, 0 },
			  { 1, 1, 1, 1, 0, 0, 0, 0 },
			  { 1, 1, 1, 0, 0, 0, 0, 0 },
			  { 1, 1, 0, 0, 0, 0, 0, 0 },
			  { 1, 0, 0, 0, 0, 0, 0, 0 } };*/
	//初始化掩码表
	_mask = vector<vector<int>>(DCT_SIZE, vector<int>(DCT_SIZE));
	_initMask(cnt);
	
	//打印掩码表
	for (unsigned i = 0; i < DCT_SIZE; ++i) {
		for (unsigned j = 0; j < DCT_SIZE; ++j)
			cout << _mask[i][j] << '\t';
		cout << endl;
	}
	//IDCT主体
	unsigned w = info_header.biWidth;
	unsigned h = info_header.biHeight;
	for (unsigned i = 0; i < h; i += DCT_SIZE) {
		for (unsigned j = 0; j < w; j += DCT_SIZE)
			_IDCT(i, j);
	}
}

void IP::DFT() {
	unsigned w = info_header.biWidth;
	unsigned h = info_header.biHeight;
	_dftvec = vector<vector<double>>(h, vector<double>(w, 0.));
	for (unsigned i = 0; i < h; i += DCT_SIZE) {
		for (unsigned j = 0; j < w; j += DCT_SIZE)
			_DFT(i, j);
	}
	//savePic(".\\pics\\air_gray_dft.bmp");
}

void IP::IDFT() {
	unsigned w = info_header.biWidth;
	unsigned h = info_header.biHeight;
	for (unsigned i = 0; i < h; i += DCT_SIZE) {
		for (unsigned j = 0; j < w; j += DCT_SIZE)
			_IDFT(i, j);
	}
	//savePic(".\\pics\\air_gray_idft.bmp");
}

vector<BYTE> IP::_get_median(const vector<tagIMAGEDATA>& vec) {
	vector<BYTE> res(3);
	vector<BYTE> bvec(9), gvec(9), rvec(9);
	for (unsigned i = 0; i < 9;++i) {
		bvec[i] = vec[i].blue;
		gvec[i] = vec[i].green;
		rvec[i] = vec[i].red;
	}
	/*
	sort(begin(bvec), end(bvec));
	sort(begin(gvec), end(gvec));
	sort(begin(rvec), end(rvec));
	
	res[0] = bvec[4];
	res[1] = gvec[4];
	res[2] = rvec[4];
	*/
	res[0] = _get_m(bvec);
	res[1] = _get_m(gvec);
	res[2] = _get_m(rvec);
	return res;
}
BYTE IP::_get_m(vector<BYTE>& vec) {
	_minmax(vec, 0, 1), _minmax(vec, 0, 2), _minmax(vec, 1, 2);
	_minmax(vec, 3, 4), _minmax(vec, 3, 5), _minmax(vec, 4, 5);
	_minmax(vec, 6, 7), _minmax(vec, 6, 8), _minmax(vec, 7, 8);
	
	_minmax(vec, 0, 6), _minmax(vec, 3, 6);
	_minmax(vec, 2, 5), _minmax(vec, 2, 8);
	
	_minmax(vec, 1, 4), _minmax(vec, 1, 7), _minmax(vec, 4, 7);
	_minmax(vec, 2, 4), _minmax(vec, 2, 6), _minmax(vec, 4, 6);

	return vec[4];
}
BYTE IP::_get_threhold() {
	BYTE res = 0;
	//计算直方图
	unsigned int gray_cnt[256] = { 0 };
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
		cur_varience = w0*pow(av_gl - av_gg, 2) + w1*pow(av_gr - av_gg, 2);
		if (cur_varience > max_varience) {
			res = BYTE(i);
			max_varience = cur_varience;
		}
	}
	return res;
}
void IP::_DCT(unsigned xoffset, unsigned yoffset) {
	double result;
	for (unsigned u = 0; u < DCT_SIZE; ++u) {
		for (unsigned v = 0; v < DCT_SIZE; ++v) {
			float EU = (u == 0 ) ? (float)(1 / sqrt(2.0)) : (float)1;
			float EV = (v == 0 ) ? (float)(1 / sqrt(2.0)) : (float)1;
			result = 0.;

			for (unsigned i = 0; i < DCT_SIZE; i++) {
				for (unsigned j = 0; j < DCT_SIZE; j++) {
					result += cos(double(2 * i + 1) * (u)* PI / (2.0*DCT_SIZE)) *
						cos(double((2 * j + 1) * (v)* PI) / (2.0*DCT_SIZE)) * (double)data[i + xoffset][j + yoffset].blue;
				}
			}
			_dctvec[xoffset + u][yoffset + v] = EU * EV * (2.0 / DCT_SIZE) * result;
			//printf("%f\t\t\t%d\n", EU * EV * 0.25 * result, BYTE(EU * EV * 0.25 * result));
			data_tmp[u + xoffset][v + yoffset].blue = BYTE(EU * EV * (2.0 / DCT_SIZE) * result);
			data_tmp[u + xoffset][v + yoffset].green = data_tmp[u + xoffset][v + yoffset].blue;
			data_tmp[u + xoffset][v + yoffset].red = data_tmp[u + xoffset][v + yoffset].blue;
		}
	}
}
void IP::_IDCT(unsigned xoffset, unsigned yoffset) {
	double result;
	for (unsigned u = 0; u < DCT_SIZE; ++u) {
		for (unsigned v = 0; v < DCT_SIZE; ++v) {
			result = 0.;

			for (unsigned i = 0; i < DCT_SIZE; ++i) {
				for (unsigned j = 0; j < DCT_SIZE; ++j) {
					float EU = (i == 0) ? (float)(1 / sqrt(2.0)) : (float)1;
					float EV = (j == 0) ? (float)(1 / sqrt(2.0)) : (float)1;
					result += EU*EV*cos((2 * (u)+1) * (i)* PI / (2.0*DCT_SIZE)) *
						cos(((2 * (v)+1) * (j)* PI) / (2.0*DCT_SIZE)) * _dctvec[i + xoffset][j + yoffset] * _mask[i][j];
				}
			}
			//printf("%f-%d\n", 0.25 * result, BYTE(0.25 * result));
			data_tmp[u + xoffset][v + yoffset].blue = BYTE((2.0 / DCT_SIZE)  * result);
			data_tmp[u + xoffset][v + yoffset].green = data_tmp[u + xoffset][v + yoffset].blue;
			data_tmp[u + xoffset][v + yoffset].red = data_tmp[u + xoffset][v + yoffset].blue;
		}
	}
}
void IP::_DFT(unsigned xoffset, unsigned yoffset) {
	double result;
	for (unsigned u = 0; u < DCT_SIZE; u++) {
		for (unsigned v = 0; v < DCT_SIZE; v++) {
			result = 0.;
			for (unsigned i = 0; i < DCT_SIZE; i++) {
				for (unsigned j = 0; j < DCT_SIZE; j++) {
					result += exp(-2.0*PI*double(u*i + v*j)/(2.0*DCT_SIZE)) * (double)data[i + xoffset][j + yoffset].blue;
				}
			}
			_dftvec[xoffset + u][yoffset + v] = result*(1.0/DCT_SIZE);
			data_tmp[u + xoffset][v + yoffset].blue = BYTE(result*(1.0 / DCT_SIZE));
			data_tmp[u + xoffset][v + yoffset].green = data_tmp[u + xoffset][v + yoffset].blue;
			data_tmp[u + xoffset][v + yoffset].red = data_tmp[u + xoffset][v + yoffset].blue;
		}
	}
}
void IP::_IDFT(unsigned xoffset, unsigned yoffset) {
	double result;
	for (unsigned u = 0; u < DCT_SIZE; u++) {
		for (unsigned v = 0; v < DCT_SIZE; v++) {
			result = 0.;
			for (unsigned i = 0; i < DCT_SIZE; i++) {
				for (unsigned j = 0; j < DCT_SIZE; j++) {
					result += exp(2.0*PI*double(u*i + v*j)/DCT_SIZE) * _dftvec[i + xoffset][j + yoffset];
				}
			}
			//printf("%d\n", BYTE(((int)result % 256)*(1.0 / 8.0)));
			data_tmp[u + xoffset][v + yoffset].blue = BYTE(result * (1.0 / DCT_SIZE));
			data_tmp[u + xoffset][v + yoffset].green = data_tmp[u + xoffset][v + yoffset].blue;
			data_tmp[u + xoffset][v + yoffset].red = data_tmp[u + xoffset][v + yoffset].blue;
		}
	}
}
void IP::_initMask(unsigned cnt) {
	if (cnt < 0 || cnt > DCT_SIZE*DCT_SIZE) {
		cout << "Wrong Mask Number!" << endl;
		return;
	}
	//Zig-Zag扫描
	unsigned _cnt = 0;
	for (unsigned i = 0; i < DCT_SIZE; ++i) {
		for (int y = i, x = 0; y >= 0; --y, ++x) {
			_mask[x][y] = 1;
			_cnt++;
			if (_cnt >= cnt) return;
		}
	}
	for (unsigned i = 1; i < DCT_SIZE; ++i) {
		for (unsigned x = i, y = 7; x < DCT_SIZE; ++x, --y) {
			_mask[x][y] = 1;
			_cnt++;
			if (_cnt >= cnt) return;
		}
	}
}
