#include <iostream>
#include <vector>
#include <stdio.h>
#include <assert.h>
#include <unordered_map>

#include "id3.h"

using namespace std;
const char lmap[] = { 'R', 'L', 'B' };
const char amap[] = { 'a', 'b', 'c', 'd' };

/*���ܣ����ֽڵ㣬�����ӽڵ㣬�ݹ����
 *���������ڵ�ָ��
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
	//���㵱ǰ�����£��������ж�����ɢֵ������չ��Ӧ�������ӽڵ�
	int has_attr_val = 0;//Ŀǰ���������У����еĸ����Ե���ɢֵ���±�
	vector<int> has_attr_vec;
	for (auto in : attr_index) {
		if (!in.empty())
			has_attr_vec.push_back(has_attr_val);
		has_attr_val++;
	}
	for (int i = 0; i < (int)has_attr_vec.size(); ++i)
		p->next.push_back(nullptr);
	
	//�ݹ�Ĺ�����
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

/*���ܣ�����ͨ��������ֵ����ȡ���ŵķ������ԡ�
 *		�� _splitNode �п���ֹͣ����
 *���������ڵ�ָ��
 *���أ�FALSE���ýڵ����Ҷ�ڵ㣻 TRUE�����»��ֳɹ�
 */
bool id3::_getBestAttr_gain(TreeNode* &p) {
	if (!p) return false;

	//����Ѿ�û�����Կɷ�
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
	
	//������е�����label��ͬ
	if (_isLeafNode(p->index)) {
		p->label = data[p->index[0]][dData - 1];
		return false;
	}

	//�������Ҷ�ڵ㣬����������Ե���ֵ
	double min_entropy = DBL_MAX;
	for (auto i:p->vecAttr) {
		double _entroy = 0;
		items _attr_index;
		//��i�����Եķֲ�����ɢֵ������
		_attr_index = _countAttr(p->index, i);

		//���������Ե�������
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

	//��һ�����ǡ�ǰ��֦��
	//���ǣ������ֵʵ�������԰��ա�������
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

/*���ܣ�ͨ��Giniָ���ķ�ʽ��������ѷ�������
 *���������ڵ�ָ��
 *���أ�false���ýڵ����Ҷ�ڵ㣻true�����»��ֽڵ�ɹ�
 */
bool id3::_getBestAttr_gini(TreeNode* &p) {
	if (!p) return false;

	//����Ѿ�û�����Կɷ�
	if (p->vecAttr.size() == 0) {
		items _label = _countAttr(p->index, dData - 1);//����label�ķֲ����
		int min_label_num = 0;
		for (int i = 0; i < attrNum[dData - 1]; ++i) {
			if ((int)_label[i].size()>min_label_num) {
				p->label = i;
				min_label_num = _label[i].size();
			}
		}
		return false;
	}

	//������е�����label��ͬ
	if (_isLeafNode(p->index)) {
		p->label = data[p->index[0]][dData - 1];
		return false;
	}

	//�������Ҷ�ڵ㣬����������Ե���ֵ
	double min_entropy = DBL_MAX;
	for (auto i : p->vecAttr) {
		double _entroy = 0;
		items _attr_index;
		//��i�����Եķֲ�����ɢֵ������
		_attr_index = _countAttr(p->index, i);

		//���������Ե�������
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

/*���ܣ��ж��Ƿ���Ҷ�ڵ�
 *�����������±�
 */
bool id3::_isLeafNode(vector<int> index) {
	if (index.empty()) return true;
	
	//�͵�һ�����ݵı�ǩ�Ƚ�
	label_t s = data[index[0]][dData - 1];

	//������ݼ����������ݱ�ǩ�͵�һ����ͬ����ô����Ҷ�ڵ�
	for (auto num : index) {
		if (data[num][dData - 1] != s)
			return false;
	}
	return true;
}

/*���ܣ��ض�index�����ڵ������ԣ���ѯ��ͬ����ɢֵ��index
 *��������ǰ�������±ꣻ����ֵ�±�
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

/*���ܣ���ͬ���ԣ���ͬ��ɢֵ����ͬlabel����Entropy
 *��������ǰ�����±꣨��ǰ�����У�ĳ���Ե���ɢֵ��ͬ��ֻ��label��ͬ��
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
	
	//����һ��Ҫע������ת��������GG��ȫ��0
	double en = 0.;
	for (auto n : label) {
		if (n == 0) continue;
		en += -(((double)n / (double)label_sum)*log2((double)n / (double)label_sum));
	}
	return en;
}

/*���ܣ���ӡ����
 *���������ڵ㣻���
 */
void id3::_printTree(TreeNode* root, int dep) {
	if (!root) return;
	if (root->label != 999) {//����������ǲ����ã����� ��=-1 ���Բ���ӡ�ڲ��ڵ�
		for (int i = 0; i < dep; ++i) {
			cout << "-";
		}
		cout << amap[root->attr]<<"("<<root->attrVal <<")" << " " << lmap[root->label]<<'\t';
		
		if (root->label != -1) {//�������ÿ��Ҷ�ڵ��������Ϣ
			for (auto in : root->index)
				cout << in+1 << " ";
		}
		cout << endl;
	}
	//�ݹ�
	for (TreeNode* p : root->next) {
		_printTree(p, dep + 1);
	}
}

/*���ܣ��Բ����������࣬�õ���ǩ
 *�������������ݼ�
 */
point id3::classify(items data) {
	int num = data.size();
	point res_label(num, 0);

	for (int i = 0; i < num; ++i) {
		res_label[i] = _getLabel(data[i]);
	}
	return res_label;
}

/*���ܣ�����һ��������ͨ�����������࣬�õ���ǩ
 *��������������
 */
int	id3::_getLabel(point data) {
	TreeNode* cur = root;//�������ƶ�
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

/*���ܣ���ȡÿ�����Ե���ɢֵ����
 *�������ޣ����ù������ݳ�Ա��
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

/*���ܣ����֦
 *�������ޣ������๲�к�����
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

/*���ܣ��ϲ�ͬһ����label��ͬ�Ľڵ�
 *�����������ڵ��ָ��
 */
void id3::_merge_node(TreeNode* &p1, TreeNode* &p2) {
	if (!p1 || !p2) return;
	if (p1->label != p2->label) return;
	for (auto n : p2->index) {
		p1->index.push_back(n);
	}
	p2 = nullptr;
}