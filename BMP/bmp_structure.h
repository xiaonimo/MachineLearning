#include <iostream>
#include <vector>

using namespace std;

typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned int	DWORD;
typedef long			LONG;

//�ļ�ͷ��Ϣ
struct  tagBITMAPFILEHEADER{
	//WORD	bfType;			//�ļ����ͣ�������0x4d42�����ַ���BM��
							//����ȥ���������ݣ������ȡʱ�����
	DWORD	bfSize;			//�����ļ���С
	WORD	bfReserved1;	//�����֣�����Ϊ0
	WORD	bfReserved2;	//�����֣�����Ϊ0
	DWORD	bfOffBits;		//���ļ�ͷ��ʵ��λͼ���ݵ�ƫ���ֽ���
};

//��Ϣͷ��Ϣ
struct tagBITMAPINFOHEADER{
	DWORD	biSize;				//��Ϣͷ��С���ֽڣ� 
	LONG	biWidth;			//ͼ����
	LONG	biHeight;			//ͼ��߶�
	WORD	biPlanes;			//λƽ����������Ϊ1
	WORD	biBitCount;			//ÿ����λ��
	DWORD	biCompression;		//ѹ�����ͣ�0��ʾ��ѹ��
	DWORD	biSizeImage;		//ѹ��ͼ���С�ֽ���
	LONG	biXPelsPerMeter;	//ˮƽ�ֱ���  
	LONG	biYPelsPerMeter;	//��ֱ�ֱ���  
	DWORD	biClrUsed;			//λͼʵ���õ���ɫ������0��ʾʹ�����е�ɫ����
	DWORD	biClrImportant;		//��ͼ����ʾ����ҪӰ�����ɫ������Ŀ�������0����ʾ����Ҫ
};  

//��ɫ����Ϣ
struct tagRGBQUAD{
	BYTE	rgbBlue;		//����ɫ����ɫ����
	BYTE	rgbGreen;		//����ɫ����ɫ����
	BYTE	rgbRed;			//����ɫ�ĺ�ɫ����
	BYTE	rgbReserved;	//����ֵ������Ϊ0
};

//������Ϣ  
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