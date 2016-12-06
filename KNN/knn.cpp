#include <iostream>
#include <vector>
#include "knn.h"

using namespace std;

/* public:
 * fit the model using train_data and train_label
 */
void KNN::fit(items _data, label _label) {
	if (_data.empty() || _label.empty()) {
		cout << "Dataset error in fit()!!!\n";
		exit(0);
	}
	return _fit(_data, _label);
}

/* public:
 * predict the test_data,return the test_label
 */
KNN::label KNN::predict(items _data) {
	if (_data.empty()) {
		cout << "Dataset error in predict()!!!\n";
		exit(0);
	}
	test_data = _data;
	if (algo == 'b')
		return _brute_predict();
	else if (algo == 't')
		return _kdtree_predict();
	else {
		cout << "The function has not been impelmented!!!\n";
		exit(0);
	}
}

/* public
 * get the accuracy score using the test_label and predict_label
 */
double KNN::accuracy_score(label _l1, label _l2) {
	if (_l1.empty() || _l2.empty()) {
		cout << "Dataset error in accuracy_score()!!!\n";
		exit(0);
	}
	return _accuracy_score(_l1, _l2);
}

/* private
 * build the model using train_data and train_label
 */
void KNN::_fit(items _data, label _label) {
	train_data = _data;
	train_label = _label;

	if (algo == 't')
		return _bulid_kdtree();
}

/* private
 * build the kdtree using train_data and train_label
 */
void KNN::_bulid_kdtree() {
	
}

/* private
 * predict the test_data using brute search, return label
 */
KNN::label KNN::_brute_predict() {
	label res;
	for (auto sample : test_data) {
		res.push_back(_get_sample_label(sample));
	}
	return res;
}

/* private:
 * predict the test_data using kd_tree, return label
 */
KNN::label KNN::_kdtree_predict() {
	label res;
	for (auto sample : test_data)
		res.push_back(_get_sample_label_kdtree(sample));
	return res;
}

/* private
 * get a point's label in brute search way
 */
KNN::label_t KNN::_get_sample_label(point p) {
	point_t res;
	for (auto sample : train_data){}
}
KNN::label_t KNN::_get_sample_label_kdtree(point) {
	
}
double KNN::_distance(point, point) {
	
}
double KNN::_accuracy_score(label, label) {
	
}