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

/* �������ݵ�֮���ŷ�Ͼ���
* ���ݵ��Ƕ�ά����
*/
double getDist(const point &x, const point &y) {
	double sum = 0;
	for (int i = 0; i < dData; ++i) {
		sum += (x[i] - y[i])*(x[i] - y[i]);
	}
	return sqrt(sum);
}
/* ������ǰ���ݵ�����
* ʹ�þ��������
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
/* ��õ�ǰ�صľ�ֵ
* �����ά��ֵ������ƽ��ֵ
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

/* ��������ؼ���ƽ�����
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
/* ��ȡ��ʼ��������
*/
void initMeans(const vector<point> &data, vector<point> &means, int k) {
	//data��ǰk��ֵ��ֵΪmeans
	for (int i = 0; i < k; ++i) {
		means[i] = data[i];
	}
	//���Բ�ͬ��means��ʼֵ�Ծ�����̵�Ӱ��
	
}
/* ��ӡ��ά������
*/
void printData(const point &x) {
	cout << endl << "(";
	for (int i = 0; i < dData; ++i) {
		cout << x[i] << ",";
	}
	cout << ")" << endl;
}
/* K-means�����㷨
*/
void kmeans(const vector<point> &data, int k) {
	//�������
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
	//means�����ʼֵ
	initMeans(data, means, k);
	//�������޴����ĵ���
	for (int i = 0; i < RCUR_CNT; ++i) {
		cout << i+1<<"th clustering..." << endl;
		//���з���
		for (int j = 0; j < (int)data.size(); ++j) {
			int label = classification(means, data[j]);
			clusters[label].push_back(data[j]);
		}
		//�����ǰ��������
		/*cout << "Means of every cluster" << endl;
		for (auto m : means)
			printData(m);
		cout << endl;*/
		//�����ǰ�ķ������
		cout << "Number of samples in every cluster:" << endl;
		for (auto cluster : clusters) {
			cout << cluster.size() << ":";
		}
		//���SSE��
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
		//����means
		for (int j = 0; j < k; ++j) {
			means[j] = getNewMean(clusters[j]);
		}
		//�����һ�η������
		//����Ҫʹ��&����clusterΪ���ã�����clear��Ч
		for (auto &cluster : clusters)
			cluster.clear();
	}
}
/* ���ļ��ж�ȡ����
*/
void getData(vector<point> &data) {
	//����ʹ��C���룬���Լӿ��ȡ���ݵ��ٶ�
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