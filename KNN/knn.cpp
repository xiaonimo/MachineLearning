#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
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
	else if (algo == 'm')
		return _template_predict();
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
	if (algo == 'b')
		;
	if (algo == 'm')
		_bulid_template();
}

/* private
 * build the kdtree using train_data and train_label
 */
void KNN::_bulid_kdtree() {
	
}

/* private
* build the digital template(mean)
*/
void KNN::_bulid_template() {
	int cnt[10] = { 0 };
	items tem(10, point(train_data[0].size()));
	unsigned train_data_sz = train_data.size();
	for (unsigned i = 0; i < train_data_sz; ++i) {
		++cnt[train_label[i]];
		unsigned point_dem_sz = train_data[0].size();
		for (unsigned j = 0; j < point_dem_sz; ++j) {
			tem[train_label[i]][j] += train_data[i][j];
		}
	}
	for (unsigned i = 0; i < 10; ++i) {
		for (unsigned j = 0; j < tem[0].size(); ++j) {
			tem[i][j] /= cnt[i];
			//cout << tem[i][j];
		}
		//cout << endl;
	}
	digital_template = tem;
}

/* private
 * predict the test_data using brute search, return label
 */
KNN::label KNN::_brute_predict() {
	label res;
	for (auto sample : test_data) {
		res.push_back(_get_sample_label_brute(sample));
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
 * predict the result using template
 */
KNN::label KNN::_template_predict() {
	label res;
	for (auto sample : test_data) {
		res.push_back(_get_sample_label_template(sample));
	}
	return res;
}

/* private
 * get a point's label in template way
 */
KNN::label_t KNN::_get_sample_label_template(point p) {
	label_t res;
	point_t min_dist = DBL_MAX;
	for (unsigned i = 0; i < 10; ++i) {
		point_t dist = _distance(p, digital_template[i]);
		if (dist < min_dist) {
			res = i;
			min_dist = dist;
		}
	}
	return res;
}

/* private
 * get a point's label in brute search way
 */
KNN::label_t KNN::_get_sample_label_brute(point p) {
	vector<pair<point_t, label_t>> que(k, make_pair(DBL_MAX, 0));
	int n_train_data = train_data.size();
	for (int i = 0; i < n_train_data; ++i) {
		point_t _dist = _distance(p, train_data[i]);
		if (_dist > que[0].first)
			continue;
		//pop from the top;
		pair<point_t, label_t> tmp1 = que[k-1];
		que[k - 1].first = DBL_MIN;
		int m = 0;
		while (2*m+1 < k-1) {
			int cm = 2 * m + 1;
			if (cm+1 < k-1 && que[cm + 1].first > que[cm].first)
				++cm;
			if (tmp1.first >= que[cm].first)
				break;
			que[m] = que[cm];
			m = cm; 
		}
		que[m] = tmp1;

		//push into the heap;
		que[k-1] = make_pair(_dist, i);
		int j = k - 1;
		pair<point_t, label_t> tmp2 = que[j];
		while (j) {
			int pj = (j-1) / 2;
			if (que[pj].first > tmp2.first)
				break;
			que[j] = que[pj];
			j = pj;
		}
		que[j] = tmp2;
	}
	label klabels;
	for (auto h : que) {
		//cout << train_label[h.second] << '\t' << h.first << endl;
		klabels.push_back(train_label[h.second]);
	}
	return _get_most_common_label(klabels);
}

/* private
 * return the most common label, use hash map
 */
KNN::label_t KNN::_get_most_common_label(label label) {
	int _res = label[0];
	unordered_map<int, int> _hmap;
	for (auto _l : label) {
		_hmap[_l]++;
		if (_hmap[_l] > _hmap[_res])
			_res = _l;
	}
	//cout << _hmap[_res] <<endl;
	return _res;
}

/* private
 * return the label from the kdtree
 */
KNN::label_t KNN::_get_sample_label_kdtree(point) {
	return label_t(0);
}

/* private
 * calculate the distance
 */
double KNN::_distance(point p1, point p2) {
	if (p1.empty() || p2.empty())  {
		cout << "Point error in _distance()!!!" << endl;
		exit(0);
	}
	int d_train_data = train_data[0].size();
	int d_test_data  = test_data[0].size();
	if (d_train_data != d_test_data) {
		cout << "Point's demension error in _distance()!!!" << endl;
		exit(0);
	}
	point_t _squre_dist = 0.;
	for (int i = 0; i < d_train_data; ++i) {
		//cout << p1[i] << '\t' << p2[i] << endl;
		_squre_dist += pow((p1[i] - p2[i]), distp);
		//cout << pow((p1[i] - p2[i]), distp) <<endl;
		//cout << (p1[i] - p2[i]) <<endl;
	}
	//cout << _squre_dist << endl;
	return pow(_squre_dist, 1.0 / distp);
}

/* private
 * calculate the accuracy
 */
double KNN::_accuracy_score(label l1, label l2) {
	if (l1.empty() || l2.empty()) {
		cout << "Label error in _accuracy_score()" << endl;
		exit(0);
	}
	if (l1.size() != l2.size()) {
		cout << "Label error in _accuracy_score()" << endl;
		exit(0);
	}
	int cnt = 0;
	for (int i = 0; i < (int)l1.size(); ++i) {
		//cout <<i << '\t' << l1[i] << '\t' << l2[i] << endl;
		if (l1[i] == l2[i])
			++cnt;
	}
	return (double)cnt / l1.size();
}



