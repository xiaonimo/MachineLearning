#include <iostream>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <unordered_map>

#include "id3.h"

using namespace std;

/*���ܣ� holdout ��֤����
 *������ԭʼ���ݣ�(part/10)�ı��������ݻ�����ѧϰ����ʣ�����Ϊ���Լ�
 */
double holdout(items data, int part) {
	double err = .0;
	part = part % 10;

	int _ndata = data.size();
	int breakpoint = (int)(_ndata*((double)part / 10.0));
	if (breakpoint >= _ndata) {
		cout << "�����»��ֻ��߻��������ݼ�" << endl;
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

/*���ܣ��ȶ� classify �����ԭʼ�����������������
 *������ԭʼ���ݣ�������
 *���أ�������
 */
double	getErrRate(items raw, point res) {
	assert(!raw.empty());
	int sum = raw.size();
	int last = raw[0].size();//����labelֵ���±�
	int cnt = 0;
	for (int i = 0; i < sum; ++i) {
		if (raw[i][last - 1] == res[i]) ++cnt;
	}
	return 1-((double)cnt / (double)sum);
}

/*���ܣ���ԭʼ���ݽ��г�ʼ����ӳ��Ϊint
 *		��ͬ���������ͣ���Ҫӳ��Ϊ������0~n,�����0~k,������������һ��
 *������ԭʼ data
 */
items getData(int nData, int dData) {
	items dataSet = items(nData, point(dData, 0));
	char label;

	freopen("data.txt", "r", stdin);
	for (int i = 0; i < nData; ++i) {

		//��ȡ
		scanf("%c,%d,%d,%d,%d", &label,
			&dataSet[i][0],
			&dataSet[i][1],
			&dataSet[i][2],
			&dataSet[i][3]);
		getchar();

		//����
		--dataSet[i][0];
		--dataSet[i][1];
		--dataSet[i][2];
		--dataSet[i][3];
		if (label == 'R') dataSet[i][4] = 0;
		else if (label == 'L') dataSet[i][4] = 1;
		else dataSet[i][4] = 2;
	}

	//���
	/*for (auto v : data) {
	for (auto n : v) {
	cout << n;
	}
	cout << endl;
	}*/
	return dataSet;
}

/*���ܣ� 10 �۽�����֤
 *������ԭʼ���ݣ�������
 *���أ�������
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
		//��֤�������ȷ��
		//cout << study_data.size() <<" "<< test_data.size() << endl;
		//ʵ��
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

/*���ܣ�bootstrap������
 *������ԭʼ����
 *���أ�������
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