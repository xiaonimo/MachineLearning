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
	int			label;			//-1:未分类，范围：0 ~ k-1
	int			attr;			//属性index，范围：0 ~ d-1
	int			attrVal;		//属性划分的离散值
	double		entropy;		//该节点的熵值
	vector<int> vecAttr;		//当前节点可以划分的属性值
	vector<int> index;			//分类或者未分类的下标
	TreeNode			*par;	//父节点
	vector<TreeNode*>	next;	//所有指向子节点的指针集合

	TreeNode():label(-1), attr(-1), attrVal(-1),par(nullptr){}
};

class id3 {
public:
	int			dData;			//数据集的数量
	int			nData;			//数据集的维度
	items		data;			//数据集
	TreeNode	*root;			//决策树的根节点
	vector<int>	attrNum;		//每个属性的离散值个数

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
	void	pre_pruning();//实现放在了 _getBestAttr_gain() 函数里
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
	double	_getEntropy(vector<int> index);//某属性的离散值相同的情况下，求Entropy
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
1.首先读入数据。
2.记录属性的维度（属性数量），记录数量，每个属性中含有多少离散值？
  每个属性的离散值，应该用一个map来记录,属性名字记录在vector里
  根据下标，可以得到属性名字
3.维护一个数据集长度大小的vector<bool>,用来记录哪些变量已经被使用？
4.树的每个节点应该定义一个结构体，结构体应该存储的信息是:
  vector<int>，被分类的数据项（如果该节点分类，如果该节点没有分类，
  那么存储的就是未分类的数据项的下标），int，该节点是否分类，如果没有分类就是-1
  分类的话就是0,1两个类别，,string,属性名字，类别名字
5.计算熵值。
*/