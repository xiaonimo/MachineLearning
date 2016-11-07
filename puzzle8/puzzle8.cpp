#include <iostream>
#include <vector>
#include <cmath>
#include <stdio.h>

using namespace std;

//空格的上下左右方向
vector<vector<int>> pos{ { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
//目标状态
vector<vector<int>> goal{ { 1, 2, 3 },
						  { 8, 0, 4 },
						  { 7, 6, 5 } };

struct TreeNode {
	int x;						//空格的x坐标
	int y;						//空格的y坐标
	vector<vector<int>> matrix;	//当前节点的状态矩阵
	struct TreeNode* pre;		//指向父节点的指针
	struct TreeNode* next;		//指向子节点的指针
	TreeNode(int _x, int _y, vector<vector<int>> _m)
		:x(_x), y(_y), matrix(_m), pre(nullptr), next(nullptr){}
};

//获取矩阵中某个元素的横纵坐标的和
int getXY(vector<vector<int>> m, int k) {
	for (int i = 0; i<3; ++i) {
		for (int j = 0; j<3; ++j)
		if (k == m[i][j])
			return i + j;
	}
	return 0;
}

//获取g值
int getG(TreeNode* p) {
	int res = 0;
	while (p) {
		++res;
		p = p->pre;
	}
	return res;
}
//获取h值,h值定义为各个数字到达目标位置的距离之和
int getH(vector<vector<int>> m) {
	int h = 0;
	for (int i = 1; i <= 9; ++i) {
		h += abs(getXY(m, i) - getXY(goal, i));
	}
	return h;
}
//获取目前矩阵状态下，下一个矩阵状态
vector<vector<int>> getNextMatrix(TreeNode* p) {
	int f = INT_MAX, g = 0, h = 0;
	int _x = 0, _y = 0;//0周围的元素的坐标，可能会越界，要做好判断
	vector<vector<int>> res_m;
	vector<vector<int>> cur_m = p->matrix;
	g = getG(p);
	//在4个方向进行选择
	for (auto po : pos) {
		_x = p->x + po[0];
		_y = p->y + po[1];

		if (_x<0 || _x>2 || _y<0 || _y>2) continue;

		swap(cur_m[p->x][p->y], cur_m[_x][_y]);
		
		//如果和父亲的父亲节点状态相同，则不采纳。
		if (p->pre && cur_m == p->pre->matrix) {
			swap(cur_m[p->x][p->y], cur_m[_x][_y]);
			continue;
		}
		
		//选择f值最小的矩阵
		h = getH(cur_m);
		if (f > g + h) {
			f = g + h;
			res_m = cur_m;
		}
		//恢复为原来的矩阵
		swap(cur_m[p->x][p->y], cur_m[_x][_y]);
	}
	return res_m;
}
//计算当前矩阵状态下空格坐标
void findZero(vector<vector<int>> m, int &x, int &y) {
	for (int i = 0; i<3; ++i) {
		for (int j = 0; j<3; ++j) {
			if (m[i][j] == 0) {
				x = i;
				y = j;
				return;
			}
		}
	}
}
//打印单独的矩阵
void printMatrix(vector<vector<int>> m) {
	for (auto v : m) {
		for (auto n : v) {
			cout << n;
		}
		cout << endl;
	}
	cout << endl;
}
//重载函数，输出链表
void printMatrix(TreeNode* p) {
	while (p) {
		printMatrix(p->matrix);
		p = p->next;
	}
}
//搜索的主体过程
void searchGoal(TreeNode* &p) {
	if (!p) return;
	int x = 0, y = 0;//新矩阵的0元素坐标
	vector<vector<int>> m;
	while (p->matrix != goal) {
		m = getNextMatrix(p);
		findZero(m, x, y);
		//构建下一个状态的节点
		TreeNode *node = new TreeNode(x, y, m);
		p->next = node;
		node->pre = p;
		p = p->next;
	}
	//回溯到初始状态节点
	while (p->pre) {
		p = p->pre;
	}
}
//得到矩阵的逆序对
int getInverseNum(vector<vector<int>> m) {
	int sum = 0;
	//转化为一维向量
	vector<int> v;
	for (auto vec : m) {
		for (auto n : vec) {
			v.push_back(n);
		}
	}
	//count逆序对
	for (int j = 1; j < 9; ++j) {
		if (v[j] < v[j-1]) ++sum;
	}
	return sum;
}
//根据逆序数来检验是否有解
bool isSolved(vector<vector<int>> m) {
	return (getInverseNum(m) + getInverseNum(goal)) % 2 == 0;
}
//检查输入的矩阵是否合法，即0-8出现且仅出现一次
bool isLegal(vector<vector<int>> m) {
	int sum = 0;
	for (auto v : m) {
		for (auto n : v)
			sum += n;
	}
	return sum == 36;
}
int main()
{
	vector<vector<int>> m{ { 2, 3, 4 },
						   { 1, 0, 5 },
						   { 8, 7, 6 } };
	if (!isLegal(m)) {
		cout << "Matrix is illeagal" << endl;
		system("pause");
		return 0;
	}
	if (!isSolved(m)) {
		cout << "Can not be solved" << endl;
		system("pause");
		return 0;
	}

	TreeNode* p = new TreeNode(1, 1, m);
	//开始搜索
	searchGoal(p);
	//打印结果
	printMatrix(p);
	
	system("pause");
	return 0;
}
