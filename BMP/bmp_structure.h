#include <iostream>
#include <vector>

using namespace std;

const double PI = 3.1415926;
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
	const unsigned					DCT_SIZE;		//dct变换size,默认8*8
	tagBITMAPFILEHEADER				file_header;	//BMP图像文件头
	tagBITMAPINFOHEADER				info_header;	//BMP图像信息头
	vector<tagRGBQUAD>				quad;			//BMP图像调色板
	vector<vector<tagIMAGEDATA>>	data;			//BMP图像位图数据
	vector<vector<tagIMAGEDATA>>	data_tmp;		//临时位图数据，所有对原图像的修改保存在该变量中

public:
	IP() :DCT_SIZE(8){}								//构造函数，默认DCT块大小是8*8
	void loadPic(const char*);						//加载图像
	void savePic(const char*);						//保存图像
	void showBmpFileHead();							//显示文件头信息
	void showBmpInfoHead();							//显示信息头信息
	void showPixels();								//显示像素信息
	tagIMAGEDATA getPixels(long int, long int);		//显示某一点的像素值（参数是像素坐标）
	void color2gray();								//彩色图  转  灰度图
	void RGB2YIQ();									//RGB图像 转  YIQ图像
	void RGB2HSI();									//RGB图像 转  HSI图像
	void RGB2XYZ();									//RGB图像 转  XYZ图像
	void RGB2YCrCb();								//RGB图像 转	 YCrBr图像
	void invide(BYTE val = 120);					//二值化（参数是阈值）
	void OSTU();									//OSTU二值化方法
	void halfTone(BYTE threhold = 0);				//halftone二值化方法
	void addSaltPeppers(unsigned int salt = 50, 
		unsigned int pepper = 50);					//添加椒盐噪声模型（参数是噪声数量）
	void addGaussian();								//添加高斯噪声（参数是）
	void medianFilter();							//中值滤波
	void meanFilter();								//均值滤波
	void convolution(vector<vector<int>>);			//卷积操作（参数是矩阵模板）
	void HistogramEqualization();					//直方图均衡化
	void HistogramMatch(vector<double> spe_pro);	//直方图规定化
	int  SumGray(unsigned x1, unsigned x2, 
		unsigned y1, unsigned y2);					//计算任意区域的像素和（参数是两点坐标，利用积分图思想）
	void DCT();										//DCT变换
	void IDCT(unsigned cnt=64);						//DCT逆变换（参数是使用的DCT系数）
	void DFT();										//DFT变换
	void IDFT();									//DFT逆变换（还没有实现）
	
private:
	int		_file_head_size;						//文件头大小
	int		_info_head_size;						//信息头大小
	WORD	_type;									//存储BMP类型（是BMP图像的话，存储0x4d42）
	int		_quad_size;								//调色板大小

	vector<vector<double>>	_dctvec;				//dct变换后的数据
	vector<vector<double>>	_dftvec;				//dft变换后的数据
	vector<vector<int>>		_mask;					//掩码，DCT逆变换用

private:
	vector<BYTE> _get_median(const vector<tagIMAGEDATA>&);
	BYTE	_get_threhold();						//求取直方图阈值（OSTU）
	void	_minmax(vector<BYTE>& vec, 
		unsigned int i, unsigned int j) {
		if (vec[i] > vec[j]) swap(vec[i], vec[j]);
	}												//交换大小值
	BYTE	_get_m(vector<BYTE>& vec);				//求取一个vector的中位数
	void	_DCT(unsigned, unsigned);				//DCT主体过程
	void	_IDCT(unsigned, unsigned);				//DCT逆变换过程
	void	_DFT(unsigned, unsigned);				//DFT主体过程
	void	_IDFT(unsigned, unsigned);				//DFT逆变换过程
	void	_initMask(unsigned cnt = 10);			//初始化掩码过程
};