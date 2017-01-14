#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "knn.h"
using namespace std;

int main() {
	/*-----read data-----*/
	vector<vector<double>>	train_data, test_data;
	vector<int>				train_label, test_label;
	train_data  = read_csv_dou(1000, 784, "trainsamples.csv");
	train_label = read_csv_int(1000, "trainlabels.csv" );
	test_data   = read_csv_dou(299, 784, "testsamples.csv");
	test_label  = read_csv_int(299, "testlabels.csv" );
	/*for (auto i:test_label)
		cout << i <<endl;
	for (auto po : test_data) {
		for (auto i : po)
			cout << i;
		cout << endl;
	}*/
	
	/*-----test-----*/
	cout << "read data completed!" << endl;
	KNN a;
	a.fit(train_data, train_label);
	cout << "begin to test" << endl;
	vector<int> p = a.predict(test_data);
	double corr = a.accuracy_score(test_label, p);
	cout << corr << endl;

	system("pause");
	return 0;
}

