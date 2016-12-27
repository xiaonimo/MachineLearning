#include <iostream>
#include <vector>

using namespace std;

const double PI = 3.1415926;
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
	const unsigned					DCT_SIZE;		//dct�任size,Ĭ��8*8
	tagBITMAPFILEHEADER				file_header;	//BMPͼ���ļ�ͷ
	tagBITMAPINFOHEADER				info_header;	//BMPͼ����Ϣͷ
	vector<tagRGBQUAD>				quad;			//BMPͼ���ɫ��
	vector<vector<tagIMAGEDATA>>	data;			//BMPͼ��λͼ����
	vector<vector<tagIMAGEDATA>>	data_tmp;		//��ʱλͼ���ݣ����ж�ԭͼ����޸ı����ڸñ�����

public:
	IP() :DCT_SIZE(8){}								//���캯����Ĭ��DCT���С��8*8
	void loadPic(const char*);						//����ͼ��
	void savePic(const char*);						//����ͼ��
	void showBmpFileHead();							//��ʾ�ļ�ͷ��Ϣ
	void showBmpInfoHead();							//��ʾ��Ϣͷ��Ϣ
	void showPixels();								//��ʾ������Ϣ
	tagIMAGEDATA getPixels(long int, long int);		//��ʾĳһ�������ֵ���������������꣩
	void color2gray();								//��ɫͼ  ת  �Ҷ�ͼ
	void RGB2YIQ();									//RGBͼ�� ת  YIQͼ��
	void RGB2HSI();									//RGBͼ�� ת  HSIͼ��
	void RGB2XYZ();									//RGBͼ�� ת  XYZͼ��
	void RGB2YCrCb();								//RGBͼ�� ת	 YCrBrͼ��
	void invide(BYTE val = 120);					//��ֵ������������ֵ��
	void OSTU();									//OSTU��ֵ������
	void halfTone(BYTE threhold = 0);				//halftone��ֵ������
	void addSaltPeppers(unsigned int salt = 50, 
		unsigned int pepper = 50);					//��ӽ�������ģ�ͣ�����������������
	void addGaussian();								//��Ӹ�˹�����������ǣ�
	void medianFilter();							//��ֵ�˲�
	void meanFilter();								//��ֵ�˲�
	void convolution(vector<vector<int>>);			//��������������Ǿ���ģ�壩
	void HistogramEqualization();					//ֱ��ͼ���⻯
	void HistogramMatch(vector<double> spe_pro);	//ֱ��ͼ�涨��
	int  SumGray(unsigned x1, unsigned x2, 
		unsigned y1, unsigned y2);					//����������������غͣ��������������꣬���û���ͼ˼�룩
	void DCT();										//DCT�任
	void IDCT(unsigned cnt=64);						//DCT��任��������ʹ�õ�DCTϵ����
	void DFT();										//DFT�任
	void IDFT();									//DFT��任����û��ʵ�֣�
	
private:
	int		_file_head_size;						//�ļ�ͷ��С
	int		_info_head_size;						//��Ϣͷ��С
	WORD	_type;									//�洢BMP���ͣ���BMPͼ��Ļ����洢0x4d42��
	int		_quad_size;								//��ɫ���С

	vector<vector<double>>	_dctvec;				//dct�任�������
	vector<vector<double>>	_dftvec;				//dft�任�������
	vector<vector<int>>		_mask;					//���룬DCT��任��

private:
	vector<BYTE> _get_median(const vector<tagIMAGEDATA>&);
	BYTE	_get_threhold();						//��ȡֱ��ͼ��ֵ��OSTU��
	void	_minmax(vector<BYTE>& vec, 
		unsigned int i, unsigned int j) {
		if (vec[i] > vec[j]) swap(vec[i], vec[j]);
	}												//������Сֵ
	BYTE	_get_m(vector<BYTE>& vec);				//��ȡһ��vector����λ��
	void	_DCT(unsigned, unsigned);				//DCT�������
	void	_IDCT(unsigned, unsigned);				//DCT��任����
	void	_DFT(unsigned, unsigned);				//DFT�������
	void	_IDFT(unsigned, unsigned);				//DFT��任����
	void	_initMask(unsigned cnt = 10);			//��ʼ���������
};