#ifndef _KMEANS_H_
#define _KMEANS_H_

#include <iostream>
#include <vector>

/*
* ²âÊÔÎÄ¼þ	test.csv						dData=784	nData=10000
*			seeds_dataset.txt				dData=8		nData=210
*			Wholesale_customers_data.csv	dData=8		nData=440
*/
#define FILEPATH	"Wholesale_customers_data.csv"
//#define dData		8
//#define nData		440
#define RCUR_CNT	100
#define RCUR_ACC	50


using namespace std;

typedef vector<double>	point;
typedef vector<point>	items;
typedef vector<items>	matrix;

class kmeans{
public:
	int		k;
	items	data;
	matrix	clusters;
public:
	kmeans(int d, int n, int _k):dData(d),nData(n),k(_k){
		data = items(nData, point(dData));
		_getData(data);
	}
	void	k_means() { clusters = _k_means(data, k); };
	void	b_kmeans() { clusters = _b_kmeans(data, k); };
	void	printClusters();
private:
	int		dData;
	int		nData;
	//string	filepath;
private:
	int		_classification(const items&, const point&);
	void	_initMeans(const items&, items&, int);
	void	_getData(items&);
	void	_printData(const point&);
	void	_printData(const items&);
	double	_getSSE(const matrix&, const items&);
	double	_getSSE(const items&, const point&);
	double	_getDist(const point&, const point&);
	point	_getMean(const items&);
	items	_getMaxSSECluster(vector<pair<double, items>>&);
	matrix	_k_means(const items&, int);
	matrix	_b_kmeans(const items&, int);
};

#endif