#include <iostream>
#include <vector>
#include <stdio.h>
#include <assert.h>
#include <unordered_map>

#include "id3.h"

using namespace std;
const char lmap[] = { 'R', 'L', 'B' };
const char amap[] = { 'a', 'b', 'c', 'd' };

/*功能：划分节点，产生子节点，递归调用
 *参数：树节点指针
 */
void id3::_splitNode(TreeNode* &p) {
	if (!_getBestAttr_gain(p)) 
		return;

	vector<vector<int>> attr_index(attrNum[p->attr]);
	//cout << data.size()<<"---------------------------"<<attr_index.size()<<endl;
	for (auto in : p->index) {
		//cout << in << " " << p->attr << " " << data[in][p->attr]<<endl;
		attr_index[data[in][p->attr]].push_back(in);
	}
	//计算当前样本下，该属性有多少离散值，并扩展相应数量的子节点
	int has_attr_val = 0;//目前的样本集中，含有的该属性的离散值的下标
	vector<int> has_attr_vec;
	for (auto in : attr_index) {
		if (!in.empty())
			has_attr_vec.push_back(has_attr_val);
		has_attr_val++;
	}
	for (int i = 0; i < (int)has_attr_vec.size(); ++i)
		p->next.push_back(nullptr);
	
	//递归的构建树
	for (int i = 0; i < (int)has_attr_vec.size(); ++i) {
		TreeNode *child = new TreeNode();
		child->index = attr_index[i];
		for (auto a : p->vecAttr) { 
			if (a == p->attr) continue;
			child->vecAttr.push_back(a);
		}
		child->par = p;
		child->attrVal = has_attr_vec[i];
		p->next[i] = child;
		_splitNode(child);
	}
}

/*功能：这里通过计算熵值，获取最优的分裂属性。
 *		在 _splitNode 中可以停止分裂
 *参数：树节点指针
 *返回：FALSE：该节点就是叶节点； TRUE：向下划分成功
 */
bool id3::_getBestAttr_gain(TreeNode* &p) {
	if (!p) return false;

	//如果已经没有属性可分
	if (p->vecAttr.size() == 0) {
		items _label = _countAttr(p->index, dData - 1);
		int min_label_num = 0;
		for (int i = 0; i < attrNum[dData - 1]; ++i) {
			if ((int)_label[i].size()>min_label_num) {
				p->label = i;
				min_label_num = _label[i].size();
			}
		}
		return false;
	}
	
	//如过所有的样本label相同
	if (_isLeafNode(p->index)) {
		p->label = data[p->index[0]][dData - 1];
		return false;
	}

	//如果不是叶节点，计算各个属性的熵值
	double min_entropy = DBL_MAX;
	for (auto i:p->vecAttr) {
		double _entroy = 0;
		items _attr_index;
		//第i个属性的分布（离散值个数）
		_attr_index = _countAttr(p->index, i);

		//求所有属性的数量和
		int attr_num_sum = 0;
		for (auto n : _attr_index) {
			attr_num_sum += n.size();
		}
		for (auto _ind : _attr_index) {
			_entroy += ((double)_ind.size() / (double)attr_num_sum)*_getEntropy(_ind);
		}
		if (_entroy < min_entropy) {
			p->attr = i;
			p->label = -1;
			min_entropy = _entroy;
		}
	}

	//这一部分是“前剪枝”
	//但是，这个阈值实在是难以把握。。。。
	/*if (p->entropy - min_entropy < 0.1) {
		items res = _countAttr(p->index, dData - 1);
		int max_sz = INT_MIN;
		for (int i = 0; i < (int)res.size(); ++i) {
			if ((int)res[i].size()>max_sz) {
				max_sz = res[i].size();
				p->label = i;
			}
		}
		return false;
	}*/
	return true;
}

/*功能：通过Gini指数的方式，计算最佳分裂属性
 *参数：树节点指针
 *返回：false，该节点就是叶节点；true，向下划分节点成功
 */
bool id3::_getBestAttr_gini(TreeNode* &p) {
	if (!p) return false;

	//如果已经没有属性可分
	if (p->vecAttr.size() == 0) {
		items _label = _countAttr(p->index, dData - 1);//计算label的分布情况
		int min_label_num = 0;
		for (int i = 0; i < attrNum[dData - 1]; ++i) {
			if ((int)_label[i].size()>min_label_num) {
				p->label = i;
				min_label_num = _label[i].size();
			}
		}
		return false;
	}

	//如过所有的样本label相同
	if (_isLeafNode(p->index)) {
		p->label = data[p->index[0]][dData - 1];
		return false;
	}

	//如果不是叶节点，计算各个属性的熵值
	double min_entropy = DBL_MAX;
	for (auto i : p->vecAttr) {
		double _entroy = 0;
		items _attr_index;
		//第i个属性的分布（离散值个数）
		_attr_index = _countAttr(p->index, i);

		//求所有属性的数量和
		int attr_num_sum = 0;
		for (auto n : _attr_index) {
			attr_num_sum += n.size();
		}
		for (auto _ind : _attr_index) {
			_entroy += ((double)_ind.size() / (double)attr_num_sum)*_getEntropy(_ind);
		}
		if (_entroy < min_entropy) {
			p->attr = i;
			p->label = -1;
			min_entropy = _entroy;
		}
	}
}

/*功能：判断是否是叶节点
 *参数：样本下标
 */
bool id3::_isLeafNode(vector<int> index) {
	if (index.empty()) return true;
	
	//和第一个数据的标签比较
	label_t s = data[index[0]][dData - 1];

	//如果数据集内所有数据标签和第一个相同，那么就是叶节点
	for (auto num : index) {
		if (data[num][dData - 1] != s)
			return false;
	}
	return true;
}

/*功能：特定index，对于单个属性，查询不同的离散值的index
 *参数：当前样本的下标；属性值下标
 */
items id3::_countAttr(vector<int> index, int attr) {
	assert(attr >= 0 && attr <= 4);
	int attr_num = attrNum[attr];
	items res;

	while (attr_num--) {	
		res.push_back(vector<int>());
	}
	for (auto num : index) {
		res[data[num][attr]].push_back(num);
	}
	return res;
}

/*功能：相同属性，相同离散值，不同label，求Entropy
 *参数：当前样本下标（当前样本中，某属性的离散值相同，只是label不同）
 */
double id3::_getEntropy(vector<int> index) {
	vector<int> label(attrNum[dData-1], 0);

	for (auto n : index) {
		label[data[n][dData - 1]]++;
	}

	int label_sum = 0;
	for (auto n : label) {
		label_sum += n;
	}
	
	//这里一定要注意类型转换，否则GG，全是0
	double en = 0.;
	for (auto n : label) {
		if (n == 0) continue;
		en += -(((double)n / (double)label_sum)*log2((double)n / (double)label_sum));
	}
	return en;
}

/*功能：打印树。
 *参数：根节点；深度
 */
void id3::_printTree(TreeNode* root, int dep) {
	if (!root) return;
	if (root->label != 999) {//这里的条件是测试用，设置 ！=-1 可以不打印内部节点
		for (int i = 0; i < dep; ++i) {
			cout << "-";
		}
		cout << amap[root->attr]<<"("<<root->attrVal <<")" << " " << lmap[root->label]<<'\t';
		
		if (root->label != -1) {//这里输出每个叶节点的样本信息
			for (auto in : root->index)
				cout << in+1 << " ";
		}
		cout << endl;
	}
	//递归
	for (TreeNode* p : root->next) {
		_printTree(p, dep + 1);
	}
}

/*功能：对测试样本分类，得到标签
 *参数：测试数据集
 */
point id3::classify(items data) {
	int num = data.size();
	point res_label(num, 0);

	for (int i = 0; i < num; ++i) {
		res_label[i] = _getLabel(data[i]);
	}
	return res_label;
}

/*功能：给定一个样本，通过决策树分类，得到标签
 *参数：单个样本
 */
int	id3::_getLabel(point data) {
	TreeNode* cur = root;//在树中移动
	while (cur->label == -1) {
		int cur_attr = cur->attr;
		for (auto child : cur->next) {
			if (child->attrVal != data[cur->attr]) {
				if (child == cur->next.back()) {
					cur = child;
					break;
				}
				continue;
			}
			cur = child;
			break;
		}
	}
	return cur->label;
}

/*功能：获取每个属性的离散值个数
 *参数：无（调用共有数据成员）
 */
point id3::_getAttrNum() {
	point res;
	for (int i = 0; i < dData; ++i) {
		unordered_map<int, int> hmap;
		for (int j = 0; j < nData; ++j) {
			hmap[data[j][i]]++;
		}
		res.push_back(hmap.size());
	}
	return res;
}

/*功能：后剪枝
 *参数：无（调用类共有函数）
 */
void id3::_post_pruning(TreeNode* p) {
	if (!p) return;
	if (p->label != -1) {
		p = p->par;
		for (int i = 0; i < p->next.size()-1; ++i) {
			for (int j = i + 1; j < p->next.size(); ++j)
				_merge_node(p->next[i], p->next[j]);
		}
		return;
	}
	for (auto child : p->next) {
		_post_pruning(child);
	}
}

/*功能：合并同一级中label相同的节点
 *参数：两个节点的指针
 */
void id3::_merge_node(TreeNode* &p1, TreeNode* &p2) {
	if (!p1 || !p2) return;
	if (p1->label != p2->label) return;
	for (auto n : p2->index) {
		p1->index.push_back(n);
	}
	p2 = nullptr;
}