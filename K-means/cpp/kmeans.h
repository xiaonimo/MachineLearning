#ifndef _KMEANS_H_
#define _KMEANS_H_

#include <iostream>
#include <vector>

#define RCUR_CNT	100		//迭代次数
#define RCUR_ACC	50		//迭代精度（最小类内距离SSE）

using namespace std;

typedef vector<double>	point;
typedef vector<point>	items;
typedef vector<items>	matrix;

class kmeans{
public:
	int		k;			//聚类数量
	items	data;		//待聚类的数据集
	matrix	clusters;	//所有的聚类，每个聚类中存储该聚类中所有点

public:
	kmeans(){}
	kmeans(items raw, int _k):data(raw),k(_k){
		dData = data[0].size();
		nData = data.size();
	}
	void	init(items raw, int _k) { _init(raw, _k); }
	void	k_means() { clusters = _k_means(data, k); }
	void	b_kmeans() { clusters = _b_kmeans(data, k); }
	void	printClusters();

private:
	int		dData;
	int		nData;

private:
	void	_init(items raw, int _k);
	int		_classification(const items&, const point&);
	void	_initMeans(const items&, items&, int);
	void	_getData(items&, const char*);
	void	_printData(const point&);
	void	_printData(const items&);
	double	_getSSE(const matrix&, const items&);
	double	_getSSE(const items&, const point&);
	double	_getDist(const point&, const point&);
	point	_getMean(const items&);
	items	_getMaxSSECluster(vector<pair<double, items>>&);
	matrix	_k_means(const items&, int);//这里的 const items& 参数需要保留，因为 _b_kmeans 会递归调用
	matrix	_b_kmeans(const items&, int);
};
items getData(int, int, const char*);
#endif