#include <iostream>
#include <stdio.h>
#include "id3.h"

using namespace std;

int main() {
	//属性离散值数量数组
	//vector<int> v{5,5,5,5,3};
	items RawData = getData(625, 5);

	//树的基本测试；
	id3 a(RawData);
	a.bulidTree();
	//a.printTree();

	//后剪枝测试
	a.post_pruning();
	a.printTree();

	//classfy功能测试
	/*items test_data{ { 1, 1, 1, 1 }, { 1, 1, 1, 2 } };
	point test_label = a.classify(test_data);
	for (auto n : test_label) cout << n;*/

	//holdout方法测试
	//cout << holdout(getData(625, 5), v);

	//验证K折交叉验证
	//KcrossValidation(RawData, 10);

	//验证bootStrap
	//cout << bootStrap(RawData,5);

	


	freopen("CON", "r", stdin);
	system("pause");
	return 0;
}

/*
开发过程中的问题：
1.分类函数死循环，因为数据集的特殊性，在分类的时候，没哟相应的类别
2.KcrossValidation在测试集取第二部分的时候，在split函数中出现vector越界bug
  但是下标都是在正常范围内，数据集也是完整的。为甚？
3.
 */