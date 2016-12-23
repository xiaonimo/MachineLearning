#include <iostream>
#include <vector>

using namespace std;

typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned int	DWORD;
typedef long			LONG;

//文件头信息
struct  tagBITMAPFILEHEADER{
	//WORD	bfType;			//文件类型，必须是0x4d42，即字符“BM”
							//这里去掉类型数据，否则读取时候出错
	DWORD	bfSize;			//整个文件大小
	WORD	bfReserved1;	//保留字，设置为0
	WORD	bfReserved2;	//保留字，设置为0
	DWORD	bfOffBits;		//从文件头到实际位图数据的偏移字节数
};

//信息头信息
struct tagBITMAPINFOHEADER{
	DWORD	biSize;				//信息头大小（字节） 
	LONG	biWidth;			//图像宽度
	LONG	biHeight;			//图像高度
	WORD	biPlanes;			//位平面数，必须为1
	WORD	biBitCount;			//每像素位数
	DWORD	biCompression;		//压缩类型，0表示不压缩
	DWORD	biSizeImage;		//压缩图像大小字节数
	LONG	biXPelsPerMeter;	//水平分辨率  
	LONG	biYPelsPerMeter;	//垂直分辨率  
	DWORD	biClrUsed;			//位图实际用到的色彩数，0表示使用所有调色板项
	DWORD	biClrImportant;		//对图像显示有重要影响的颜色索引数目，如果是0，表示都重要
};  

//调色板信息
struct tagRGBQUAD{
	BYTE	rgbBlue;		//该颜色的蓝色分量
	BYTE	rgbGreen;		//该颜色的绿色分量
	BYTE	rgbRed;			//该颜色的红色分量
	BYTE	rgbReserved;	//保留值，设置为0
};

//像素信息  
struct tagIMAGEDATA {
	BYTE	blue;
	BYTE	green;  
	BYTE	red;  
};

class IP{
public:
	tagBITMAPFILEHEADER				file_header;
	tagBITMAPINFOHEADER				info_header;
	vector<tagRGBQUAD>				quad;
	vector<vector<tagIMAGEDATA>>	data;
	vector<vector<tagIMAGEDATA>>	data_tmp;

public:
	void loadPic(const char*);
	void savePic(const char*);
	void showBmpFileHead();
	void showBmpInfoHead();
	void showPixels();
	tagIMAGEDATA getPixels(long int, long int);
	void colorInvert(unsigned char maxPixel = 255);
	void RGB2YIQ(const char*);
	void RGB2HSI(const char*);
	void RGB2XYZ(const char*);
	void invide(const char*, BYTE val=120);
	void OSTU(const char*);
	void addSalt(unsigned int salt = 50, unsigned int pepper = 50);
	void addGaussian(unsigned int g=100);

private:
	int		_file_head_size;
	int		_info_head_size;
	WORD	_type;
	int		_quad_size;

private:

};