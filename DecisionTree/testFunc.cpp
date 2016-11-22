#include <iostream>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <unordered_map>

#include "id3.h"

using namespace std;

/*功能： holdout 验证方法
 *参数：原始数据；(part/10)的比例的数据会用作学习集，剩余的作为测试集
 */
double holdout(items data, int part) {
	double err = .0;
	part = part % 10;

	int _ndata = data.size();
	int breakpoint = (int)(_ndata*((double)part / 10.0));
	if (breakpoint >= _ndata) {
		cout << "请重新划分或者换其他数据集" << endl;
		return err;
	}
	items study_data(begin(data), begin(data) + breakpoint);
	items test_data(begin(data) + breakpoint, end(data));

	id3 a(study_data);
	a.bulidTree();
	//a.printTree();

	vector<int> test_res = a.classify(test_data);

	err = getErrRate(test_data, test_res);
	return err;
}

/*功能：比对 classify 结果和原始分类结果，计算错误率
 *参数：原始数据；分类结果
 *返回：错误率
 */
double	getErrRate(items raw, point res) {
	assert(!raw.empty());
	int sum = raw.size();
	int last = raw[0].size();//储存label值得下标
	int cnt = 0;
	for (int i = 0; i < sum; ++i) {
		if (raw[i][last - 1] == res[i]) ++cnt;
	}
	return 1-((double)cnt / (double)sum);
}

/*功能：对原始数据进行初始化，映射为int
 *		不同的数据类型，需要映射为属性是0~n,类别是0~k,而且类别在最后一列
 *参数：原始 data
 */
items getData(int nData, int dData) {
	items dataSet = items(nData, point(dData, 0));
	char label;

	freopen("data.txt", "r", stdin);
	for (int i = 0; i < nData; ++i) {

		//读取
		scanf("%c,%d,%d,%d,%d", &label,
			&dataSet[i][0],
			&dataSet[i][1],
			&dataSet[i][2],
			&dataSet[i][3]);
		getchar();

		//处理
		--dataSet[i][0];
		--dataSet[i][1];
		--dataSet[i][2];
		--dataSet[i][3];
		if (label == 'R') dataSet[i][4] = 0;
		else if (label == 'L') dataSet[i][4] = 1;
		else dataSet[i][4] = 2;
	}

	//输出
	/*for (auto v : data) {
	for (auto n : v) {
	cout << n;
	}
	cout << endl;
	}*/
	return dataSet;
}

/*功能： 10 折交叉验证
 *参数：原始数据；分类结果
 *返回：错误率
 */
double KcrossValidation(items raw, int k) {
	int _ndata = raw.size();
	assert(_ndata > k);
	int part = _ndata / k;

	vector<double> err;
	for (int i = 1; i <= k; ++i) {
		items study_data;
		items test_data;
		for (int j = 0; j < _ndata; ++j) {
			if (j >= (i - 1)*part && j < i*part) {
				//cout << j << endl;
				test_data.push_back(raw[j]);
			}
			else {
				study_data.push_back(raw[j]);
			}
		}
		//验证分类的正确性
		//cout << study_data.size() <<" "<< test_data.size() << endl;
		//实现
		id3 a(study_data);
		//cout << a.nData << " " << a.dData << " " << a.root->attr << endl;
		a.bulidTree();
		vector<int> test_res = a.classify(test_data);

		err.push_back(getErrRate(test_data, test_res));
	}

	double errSum = 0.0;
	for (auto e : err) {
		cout << e << '\t';
		errSum += e;
	}
	cout << endl;
	return errSum / err.size();
}

/*功能：bootstrap自助法
 *参数：原始数据
 *返回：错误率
 */
double bootStrap(items raw,int cnt) {
	
	int _ndata = raw.size();
	double err = 0.;
	vector<double> errVec;
	for (cnt = 0; cnt < 5; ++cnt){
		unordered_map<int, int> hmap;
		items study_data;
		items test_data;

		srand((unsigned)cnt);
		for (int i = 0; i < _ndata; ++i) {
			int index = rand() % _ndata;
			hmap[index]++;
			study_data.push_back(raw[i]);
		}

		for (int i = 0; i < _ndata; ++i) {
			if (hmap[i]) continue;
			test_data.push_back(raw[i]);
		}
		cout << study_data.size() << " " << test_data.size() << endl;
		id3 a(study_data);
		a.bulidTree();
		//a.printTree();

		vector<int> test_res = a.classify(test_data);
		err = getErrRate(test_data, test_res);
		errVec.push_back(err);
	}
	double errSum = .0;
	for (auto e : errVec) {
		errSum = e*0.632 + 0.368*(1 - e);
	}
	return errSum/cnt;
}