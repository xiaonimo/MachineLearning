#define	_CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <stdio.h>
#include "knn.h"
using namespace std;

/* read data from the csv file
*/
vector<vector<double>> read_csv_dou(unsigned N, unsigned D, const char* filename) {
	vector<vector<double>> res(N, vector<double>(D));
	freopen(filename, "r", stdin);
	for (auto &point : res) {
		for (auto &attr : point)
			scanf("%lf,", &attr);
	}
	fclose(stdin);
	freopen("CON", "r", stdin);
	return res;
}

vector<int> read_csv_int(unsigned N, const char* filename) {
	vector<int> res(N);
	freopen(filename, "r", stdin);
	/*for (auto &point : res) {
		scanf("%d\n", &point);
	}*/
	for (unsigned i = 0; i < N; ++i) {
		scanf("%d,", &res[i]);
	}
	fclose(stdin);
	freopen("CON", "r", stdin);
	return res;
}
