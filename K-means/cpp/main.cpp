#include <iostream>
#include "kmeans.h"

using namespace std;
/*
* �����ļ�	test.csv						dData=784	nData=10000
*			seeds_dataset.txt				dData=8		nData=210
*			Wholesale_customers_data.csv	dData=8		nData=440
*/
int main() {

	/*��ȡ���ݲ���*/
	items raw_data = getData(210, 8, "seeds_dataset.txt");
	
	/*Ĭ�Ϲ��캯������*/
	//kmeans c;
	//c.init(raw_data, 10);

	/*���������캯������*/
	kmeans c(raw_data, 8);
	
	/*kmeans�㷨����*/
	//c.k_means();

	/*����kmeans�㷨����*/
	c.b_kmeans();

	/*��ӡ����������*/
	c.printClusters();
	
	freopen("CON", "r", stdin);
	system("pause");
	return 0;
}