#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <unordered_map>

#include "kmeans.h"

using namespace std;

/* ���ܣ���ʼ�����󣨶���ͨ���޲������캯��������
 * ������ԭʼ����
 */
void kmeans::_init(items raw, int _k) {
	data = raw;
	nData = data.size();
	dData = data[0].size();
	k = _k;
}

/* ���ܣ��������ݵ�֮���ŷ�Ͼ���
 * ����������������
 */
double kmeans::_getDist(const point &x, const point &y) {
	double sum = 0.;
	for (int i = 0; i < dData; ++i) {
		sum += abs(x[i] - y[i])*abs(x[i] - y[i]);
	}
	assert(sum >= 0);
	return sqrt(sum);
}

/* ���ܣ��ж���ǰ���ݵ�����
 * ���������о�ֵ�㣬���ж���
 */
int kmeans::_classification(const items &means, const point &x) {
	int sz = means.size();
	int label = 0;
	double minDist = DBL_MAX;
	for (int i = 0; i < sz; ++i) {
		double dist = _getDist(means[i], x);
		if (dist < minDist) {
			minDist = dist;
			label = i;
		}
	}
	return label;
}

/* ���ܣ���õ�ǰ�صľ�ֵ
 * �����������ؼ�
 */
point kmeans::_getMean(const items &cluster) {
	int sz = cluster.size();
	assert(sz > 0);
	point newMean(dData, 0);
	for (int i = 0; i < dData; ++i) {
		for (int j = 0; j < sz; ++j) {
			newMean[i] += cluster[j][i];
		}
		newMean[i] /= sz;
	}
	return newMean;
}

/* ���ܣ��������дؼ���ƽ�����
 * ���������дؼ������дؼ��ľ�ֵ��Ҫ�±��Ӧ��
 */
double kmeans::_getSSE(const matrix &clusters, const items &means) {
	double res = 0;
	int i = 0;
	for (const auto &c : clusters) {
		res += _getSSE(c, means[i]);//������һ�����غ���
		++i;
	}
	return res;
}

/* ���ܣ���������ؼ���ƽ�����
 * �����������ؼ����ôؼ��ؼ��ľ�ֵ
 */
double kmeans::_getSSE(const items &cluster, const point &mean) {
	double res = 0;
	for (auto p : cluster) {
		res += _getDist(p, mean);
	}
	return res;
}

/* ���ܣ���ȡ��ʼ�������ģ�ǰK������ֵ��ʼ��Ϊ��������
 * ����������ֵ���������ı������������ĸ���
 */
void kmeans::_initMeans(const items &data, items &means, int k) {
	//data��ǰk��ֵ��ֵΪmeans
	for (int i = 0; i < k; ++i) {
		means[i] = data[i];
	}
	//�����ʼ����������
	//���Բ�ͬ��means��ʼֵ�Ծ�����̵�Ӱ��
	/*unordered_map<int, int> hmap;
	for (int i = 0; i < k;) {
		srand((unsigned)time(nullptr)+(unsigned)i);
		int _index = rand() % data.size();
		if (hmap[_index]) continue;

		means[i] = data[_index];
		hmap[_index]++;
		++i;
	}*/
}

/* ���ܣ���ӡ�����ݣ���ά�ȣ�
 * ��������
 */
void kmeans::_printData(const point &x) {
	cout << endl << "(";
	for (int i = 0; i < dData; ++i) {
		cout << x[i] << ",";
	}
	cout << ")" << endl;
}

/* ���ܣ���ӡ�ؼ����ݣ���㼯�ϣ�
 * �������ؼ�
 */
void kmeans::_printData(const items &x) {
	for (auto p : x)
		_printData(p);
}

/* ����:k-means�����㷨
 * ���������ݼ�����������
 */
matrix kmeans::_k_means(const items &data, int k) {
	//�������
	if (data.empty()) {
		cout << "DATA IS WRONG!!!" << endl;
		return matrix();
	}
	if (k == 0) {
		cout << "K IS WRONG!!!" << endl;
		return matrix();
	}
	
	matrix	_clusters(k);
	items	means(k);
	double	sse = 0, osse = 0;

	//means�����ʼֵ
	_initMeans(data, means, k);
	//�������޴����ĵ���
	for (int i = 0; i < RCUR_CNT; ++i) {
		cout << i + 1 << "th clustering..." << endl;
		//���з���
		for (int j = 0; j < (int)data.size(); ++j) {
			int label = _classification(means, data[j]);
			_clusters[label].push_back(data[j]);
		}
		//�����ǰ��������
		/*cout << "Means of every cluster" << endl;
		for (auto m : means)
		printData(m);
		cout << endl;*/

		//�����ǰ�ķ������
		/*cout << "Number of samples in every cluster:" << endl;
		for (auto cluster : _clusters) {
			cout << cluster.size() << ":";
		}*/

		//����SSE��
		sse = _getSSE(_clusters, means);
		if (abs(osse - sse) < RCUR_ACC) {
			//cout << "K-means completed!" << endl;
			//freopen("CON", "r", stdin);
			//system("pause");
			return _clusters;
		}
		//cout << endl << "==================================" << endl;
		osse = sse;

		//����means
		for (int j = 0; j < k; ++j) {
			means[j] = _getMean(_clusters[j]);
		}

		//�����һ�η������
		//����Ҫʹ��&����clusterΪ���ã�����clear��Ч
		for (auto &cluster : _clusters)
			cluster.clear();
	}
	return _clusters;
}

/* ���ܣ����ڶ��־��࣬ѡ��Ŀǰ���о�����SSE���ľ���
 * �������洢pair<sse,cluster>��vector
 */
items kmeans::_getMaxSSECluster(vector<pair<double, items>> &vec) {
	if (vec.empty()) {
		cout << "getMaxSSECluster is wrong!!!";
		system("pause");
	}
	items res;
	double maxSSE = 0.;
	auto itr = begin(vec);
	for (auto beg = vec.begin(); beg != vec.end(); ++beg) {
		if ((*beg).first > maxSSE) {
			res = (*beg).second;
			itr = beg;
			maxSSE = (*beg).first;
		}
	}
	vec.erase(itr);
	return res;
}

/* ���ܣ�����k-means����
 * ���������ݼ�����������
 */
matrix kmeans::_b_kmeans(const items &data, int k) {
	if (data.empty()) {
		cout << "DATA IS WRONG!!!" << endl;
		return matrix();
	}
	if (k == 0) {
		cout << "K IS WRONG!!!" << endl;
		return matrix();
	}
	if (k == 1)
		return matrix(1, data);

	matrix _clusters;
	vector<pair<double, items>> vec;

	vec.push_back(make_pair(_getSSE(data, _getMean(data)), data));
	
	//����������k=2��kmeans�㷨
	for (int i = 0; i < k; ++i) {
		items maxSSEItems = _getMaxSSECluster(vec);
		matrix res = _k_means(maxSSEItems, 2);
		for (const auto &cluster : res) {
			vec.push_back(make_pair(_getSSE(cluster, _getMean(cluster)), cluster));
		}
		for (const auto &p : vec) {
			cout << "***" << p.second.size();
		}
		cout << endl;
	}
	//��pair�л�ȡ���о���
	for (const auto &p : vec) {
		_clusters.push_back(p.second);
	}
	return _clusters;
}

/* ���ܣ���ȡ���ݣ�����ʹ����c����
 * ���������ݼ����������ã��ñ�����ǰ���붨��
 */
void kmeans::_getData(items &data, const char* filename) {
	//����ʹ��C���룬���Լӿ��ȡ���ݵ��ٶ�
	freopen(filename, "r", stdin);
	for (auto &point_data : data) {
		for (int i = 0; i < dData; ++i) {
			scanf("%lf,", &point_data[i]);
		}
	}
	fclose(stdin);
}

/* ���ܣ���ӡ���յľ�����
 * ������clusters
 */
void kmeans::printClusters() {
	cout << endl << "The results of clustering:" << endl;
	double sse = .0;
	//cout << "SSE = " << _getSSE(clusters, means);
	for (const auto& clu : clusters) {
		cout << clu.size() << "-";
		sse += _getSSE(clu, _getMean(clu));
	}
	cout << endl<< "SSE = " << sse << endl;
}

/*���ܣ����������ⲿ�����ݶ�ȡ���ܣ�
 *���������ݵ�ά����Ϣ
 */
items getData(int _nData, int _dData, const char* filename) {
	//����ʹ��C���룬���Լӿ��ȡ���ݵ��ٶ�
	items res(_nData, point(_dData));
	freopen(filename, "r", stdin);
	for (auto &point_data : res) {
		for (int i = 0; i < _dData; ++i) {
			scanf("%lf,", &point_data[i]);
		}
	}
	fclose(stdin);
	return res;
}
