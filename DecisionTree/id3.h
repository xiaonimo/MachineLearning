#include <iostream>
#include <vector>
#include <map>

using namespace std;

typedef vector<int>		point;
typedef vector<point>	items;
typedef vector<items>	matrix;

typedef int label_t;

struct TreeNode{
public:
	int			label;			//-1:δ���࣬��Χ��0 ~ k-1
	int			attr;			//����index����Χ��0 ~ d-1
	int			attrVal;		//���Ի��ֵ���ɢֵ
	double		entropy;		//�ýڵ����ֵ
	vector<int> vecAttr;		//��ǰ�ڵ���Ի��ֵ�����ֵ
	vector<int> index;			//�������δ������±�
	TreeNode			*par;	//���ڵ�
	vector<TreeNode*>	next;	//����ָ���ӽڵ��ָ�뼯��

	TreeNode():label(-1), attr(-1), attrVal(-1),par(nullptr){}
};

class id3 {
public:
	int			dData;			//���ݼ�������
	int			nData;			//���ݼ���ά��
	items		data;			//���ݼ�
	TreeNode	*root;			//�������ĸ��ڵ�
	vector<int>	attrNum;		//ÿ�����Ե���ɢֵ����

public:
	id3(items rawData) :data(rawData){
		nData = data.size();
		dData = data[0].size();
		attrNum = _getAttrNum();
		_initRoot();
	}

	void	bulidTree(){ _splitNode(root); };
	void	printTree(){ _printTree(root,0); };
	point	classify(items data);
	void	pre_pruning();//ʵ�ַ����� _getBestAttr_gain() ������
	void	post_pruning() { _post_pruning(root); };
private:
	

private:
	void	_initRoot() {
		root = new TreeNode();
		for (int i = 0; i < nData; ++i) root->index.push_back(i);
		for (int i = 0; i < dData - 1; ++i) root->vecAttr.push_back(i);

		items ca = _countAttr(root->index, dData - 1);
		int sz = 0;
		double en = 0;
		for (auto c : ca) sz += c.size();
		for (auto c : ca) en += -((double)c.size() / sz)*log2((double)c.size() / sz);
		root->entropy = en;
	};
	point	_getAttrNum();
	bool	_getBestAttr_gain(TreeNode* &p);
	items	_countAttr(vector<int> index, int attr);
	bool	_isLeafNode(vector<int>);
	void	_createNode(TreeNode* p);
	double	_getEntropy(vector<int> index);//ĳ���Ե���ɢֵ��ͬ������£���Entropy
	//void	_getData(items&);
	void	_splitNode(TreeNode* &p);
	void	_printTree(TreeNode*, int);
	int		_getLabel(point);
	void	_post_pruning(TreeNode*);
	void	_merge_node(TreeNode* &p1, TreeNode* &p2);
};

items	getData(int nData, int dData);
double	holdout(items, int part=8);
double	getErrRate(items raw, point res);
double	KcrossValidation(items raw, int k);
double	bootStrap(items raw,int cnt);
/*
1.���ȶ������ݡ�
2.��¼���Ե�ά�ȣ���������������¼������ÿ�������к��ж�����ɢֵ��
  ÿ�����Ե���ɢֵ��Ӧ����һ��map����¼,�������ּ�¼��vector��
  �����±꣬���Եõ���������
3.ά��һ�����ݼ����ȴ�С��vector<bool>,������¼��Щ�����Ѿ���ʹ�ã�
4.����ÿ���ڵ�Ӧ�ö���һ���ṹ�壬�ṹ��Ӧ�ô洢����Ϣ��:
  vector<int>������������������ýڵ���࣬����ýڵ�û�з��࣬
  ��ô�洢�ľ���δ�������������±꣩��int���ýڵ��Ƿ���࣬���û�з������-1
  ����Ļ�����0,1�������,string,�������֣��������
5.������ֵ��
*/