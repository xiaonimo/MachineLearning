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

/* 功能：初始化对象（对象通过无参数构造函数建立）
 * 参数：原始数据
 */
void kmeans::_init(items raw, int _k) {
	data = raw;
	nData = data.size();
	dData = data[0].size();
	k = _k;
}

/* 功能：计算数据点之间的欧氏距离
 * 参数：两个点坐标
 */
double kmeans::_getDist(const point &x, const point &y) {
	double sum = 0.;
	for (int i = 0; i < dData; ++i) {
		sum += abs(x[i] - y[i])*abs(x[i] - y[i]);
	}
	assert(sum >= 0);
	return sqrt(sum);
}

/* 功能：判定当前数据点的组别
 * 参数：所有均值点，待判定点
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

/* 功能：获得当前簇的均值
 * 参数：整个簇集
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

/* 功能：计算所有簇集的平方误差
 * 参数：所有簇集，所有簇集的均值（要下标对应）
 */
double kmeans::_getSSE(const matrix &clusters, const items &means) {
	double res = 0;
	int i = 0;
	for (const auto &c : clusters) {
		res += _getSSE(c, means[i]);//调用另一个重载函数
		++i;
	}
	return res;
}

/* 功能：计算给定簇集的平方误差
 * 参数：单个簇集，该簇集簇集的均值
 */
double kmeans::_getSSE(const items &cluster, const point &mean) {
	double res = 0;
	for (auto p : cluster) {
		res += _getDist(p, mean);
	}
	return res;
}

/* 功能：获取初始聚类中心，前K个数据值初始化为聚类中心
 * 参数：数据值，聚类中心变量，聚类中心个数
 */
void kmeans::_initMeans(const items &data, items &means, int k) {
	//data的前k个值赋值为means
	for (int i = 0; i < k; ++i) {
		means[i] = data[i];
	}
	//随机初始化聚类中心
	//测试不同的means初始值对聚类过程的影响
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

/* 功能：打印点数据（多维度）
 * 参数：点
 */
void kmeans::_printData(const point &x) {
	cout << endl << "(";
	for (int i = 0; i < dData; ++i) {
		cout << x[i] << ",";
	}
	cout << ")" << endl;
}

/* 功能：打印簇集数据（多点集合）
 * 参数：簇集
 */
void kmeans::_printData(const items &x) {
	for (auto p : x)
		_printData(p);
}

/* 功能:k-means主体算法
 * 参数：数据集，聚类数量
 */
matrix kmeans::_k_means(const items &data, int k) {
	//参数检查
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

	//means赋予初始值
	_initMeans(data, means, k);
	//进行有限次数的迭代
	for (int i = 0; i < RCUR_CNT; ++i) {
		cout << i + 1 << "th clustering..." << endl;
		//进行分类
		for (int j = 0; j < (int)data.size(); ++j) {
			int label = _classification(means, data[j]);
			_clusters[label].push_back(data[j]);
		}
		//输出当前聚类中心
		/*cout << "Means of every cluster" << endl;
		for (auto m : means)
		printData(m);
		cout << endl;*/

		//输出当前的分类情况
		/*cout << "Number of samples in every cluster:" << endl;
		for (auto cluster : _clusters) {
			cout << cluster.size() << ":";
		}*/

		//计算SSE：
		sse = _getSSE(_clusters, means);
		if (abs(osse - sse) < RCUR_ACC) {
			//cout << "K-means completed!" << endl;
			//freopen("CON", "r", stdin);
			//system("pause");
			return _clusters;
		}
		//cout << endl << "==================================" << endl;
		osse = sse;

		//更新means
		for (int j = 0; j < k; ++j) {
			means[j] = _getMean(_clusters[j]);
		}

		//清空上一次分类情况
		//这里要使用&设置cluster为引用，否则clear无效
		for (auto &cluster : _clusters)
			cluster.clear();
	}
	return _clusters;
}

/* 功能：用于二分聚类，选择目前所有聚类中SSE最大的聚类
 * 参数：存储pair<sse,cluster>的vector
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

/* 功能：二分k-means聚类
 * 参数：数据集，聚类数量
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
	
	//迭代，调用k=2的kmeans算法
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
	//从pair中获取所有聚类
	for (const auto &p : vec) {
		_clusters.push_back(p.second);
	}
	return _clusters;
}

/* 功能：读取数据，这里使用了c代码
 * 参数：数据集变量的引用，该变量提前申请定义
 */
void kmeans::_getData(items &data, const char* filename) {
	//这里使用C代码，可以加快读取数据的速度
	freopen(filename, "r", stdin);
	for (auto &point_data : data) {
		for (int i = 0; i < dData; ++i) {
			scanf("%lf,", &point_data[i]);
		}
	}
	fclose(stdin);
}

/* 功能：打印最终的聚类结果
 * 参数：clusters
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

/*功能：定义在类外部的数据读取功能，
 *参数：数据的维度信息
 */
items getData(int _nData, int _dData, const char* filename) {
	//这里使用C代码，可以加快读取数据的速度
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
