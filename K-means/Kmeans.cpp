#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>

#define FILEPATH "test.csv"
#define dData 784
#define RCUR_CNT 100
#define RCUR_ACC 50

using namespace std;

typedef vector<int> point;

/* 计算数据点之间的欧氏距离
* 数据点是二维数据
*/
double getDist(const point &x, const point &y) {
	double sum = 0;
	for (int i = 0; i < dData; ++i) {
		sum += (x[i] - y[i])*(x[i] - y[i]);
	}
	return sqrt(sum);
}
/* 决定当前数据点的组别
* 使用距离分类器
*/
int classification(const vector<point> &means, const point &x) {
	int sz = means.size();
	int label = 0;
	double minDist = (double)INT_MAX;
	for (int i = 0; i < sz; ++i) {
		double dist = getDist(means[i], x);
		if (dist < minDist) {
			minDist = dist;
			label = i;
		}
	}
	return label;
}
/* 获得当前簇的均值
* 求各个维度值的算数平均值
*/
point getNewMean(const vector<point> &cluster) {
	int sz = cluster.size();
	point newMean(dData, 0);
	for (int i = 0; i < dData; ++i) {
		for (int j = 0; j < sz; ++j) {
			newMean[i] += cluster[j][i];
		}
		newMean[i] /= sz;
	}
	return newMean;
}

/* 计算给定簇集的平方误差
*/
double getSSE(const vector<vector<point>> &clusters, const vector<point> &means) {
	double res = 0;
	int i = 0;
	//int clusters_sz = clusters.size();
	//for (int i = 0; i < clusters_sz; ++i) {
	//	for (int j = 0; j < dData; ++j) {
	//		//overflow!!!
	//		res += getDist(clusters[i][j], means[i]);
	//	}
	//}
	for (const auto &c : clusters) {
		for (const auto &p : c) {
			res += getDist(p, means[i]);
		}
		++i;
	}
	return res;
}
/* 获取初始聚类中心
*/
void initMeans(const vector<point> &data, vector<point> &means, int k) {
	//data的前k个值赋值为means
	for (int i = 0; i < k; ++i) {
		means[i] = data[i];
	}
	//测试不同的means初始值对聚类过程的影响
	
}
/* 打印多维度数据
*/
void printData(const point &x) {
	cout << endl << "(";
	for (int i = 0; i < dData; ++i) {
		cout << x[i] << ",";
	}
	cout << ")" << endl;
}
/* K-means主体算法
*/
void kmeans(const vector<point> &data, int k) {
	//参数检查
	if (data.empty()) {
		cout << "DATA IS WRONG!!!" << endl;
		return;
	}
	if (k == 0) {
		cout << "K IS WRONG!!!" << endl;
		return;
	}
	vector<vector<point>> clusters(k);
	vector<point> means(k);
	double sse = 0, osse = 0;
	//means赋予初始值
	initMeans(data, means, k);
	//进行有限次数的迭代
	for (int i = 0; i < RCUR_CNT; ++i) {
		cout << i+1<<"th clustering..." << endl;
		//进行分类
		for (int j = 0; j < (int)data.size(); ++j) {
			int label = classification(means, data[j]);
			clusters[label].push_back(data[j]);
		}
		//输出当前聚类中心
		/*cout << "Means of every cluster" << endl;
		for (auto m : means)
			printData(m);
		cout << endl;*/
		//输出当前的分类情况
		cout << "Number of samples in every cluster:" << endl;
		for (auto cluster : clusters) {
			cout << cluster.size() << ":";
		}
		//输出SSE：
		sse = getSSE(clusters, means);
		cout << osse - sse << endl;
		if (abs(osse - sse) < RCUR_ACC) {
			cout << "K-means completed!" << endl;
			freopen("CON", "r", stdin);
			system("pause");
			return;
		}
		osse = sse;

		cout << endl << "==================================" << endl;
		osse = sse;
		//更新means
		for (int j = 0; j < k; ++j) {
			means[j] = getNewMean(clusters[j]);
		}
		//清空上一次分类情况
		//这里要使用&设置cluster为引用，否则clear无效
		for (auto &cluster : clusters)
			cluster.clear();
	}
}
/* 在文件中读取数据
*/
void getData(vector<point> &data) {
	//这里使用C代码，可以加快读取数据的速度
	freopen(FILEPATH, "r", stdin);
	for (auto &point_data:data) {
		for (int i = 0; i < dData; ++i) {
			scanf("%d,", &point_data[i]);
		}
	}
	fclose(stdin);
}
int main() {
	vector<point> data(10000, point(784));
	cout << "Reading data..." << endl;
	getData(data);
	cout << "Read data finished!" << endl;
	kmeans(data, 10);
	
	system("pause");
	return 0;
}