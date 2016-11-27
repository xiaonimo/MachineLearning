#include <iostream>
#include "kmeans.h"

using namespace std;
/*
* 测试文件	test.csv						dData=784	nData=10000
*			seeds_dataset.txt				dData=8		nData=210
*			Wholesale_customers_data.csv	dData=8		nData=440
*/
int main() {

	/*读取数据测试*/
	items raw_data = getData(210, 8, "seeds_dataset.txt");
	
	/*默认构造函数测试*/
	//kmeans c;
	//c.init(raw_data, 10);

	/*带参数构造函数测试*/
	kmeans c(raw_data, 8);
	
	/*kmeans算法测试*/
	//c.k_means();

	/*二分kmeans算法测试*/
	c.b_kmeans();

	/*打印聚类结果测试*/
	c.printClusters();
	
	freopen("CON", "r", stdin);
	system("pause");
	return 0;
}