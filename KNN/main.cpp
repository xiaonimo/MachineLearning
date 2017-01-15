#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "knn.h"
#include <time.h>
using namespace std;

int main() {
	/*-----read data-----*/
	vector<vector<double>>	train_data, test_data;
	vector<int>				train_label, test_label;
	train_data  = read_csv_dou(1000, 784, "trainsamples.csv");
	train_label = read_csv_int(1000, "trainlabels.csv" );
	test_data   = read_csv_dou(299, 784, "testsamples.csv");
	test_label  = read_csv_int(299, "testlabels.csv" );
	
	clock_t start_t, finish_t;
	/*-----test-----*/
	start_t = clock();
	cout << "read data completed!" << endl;
	KNN a(10, 2, 'm');
	a.fit(train_data, train_label);
	cout << "begin to test" << endl;
	vector<int> p = a.predict(test_data);
	double corr = a.accuracy_score(test_label, p);
	finish_t = clock();
	cout << "Accuracy=" << corr << endl;
	cout << "Time=" << double(finish_t-start_t)/1000 << endl;


	system("pause");
	return 0;
}

