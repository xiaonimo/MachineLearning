#include <iostream>
#include "kmeans.h"

using namespace std;

int main() {
	kmeans c(8,440,10);
	c.k_means();
	c.printClusters();
	
	freopen("CON", "r", stdin);
	system("pause");
	return 0;
}