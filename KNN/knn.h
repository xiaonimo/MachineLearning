#ifndef _KNN_H_
#define _KNN_H_
#include <iostream>
#include <vector>

using namespace std;

class KNN{
	typedef double			point_t;
	typedef vector<point_t> point;
	typedef vector<point>	items;
	typedef int				label_t;
	typedef vector<label_t> label;
	typedef double(*f_dist)(point, point);

public:
	items		train_data;
	items		test_data;
	label		train_label;
	label		test_label;

public:
	KNN(int _k = 20, int _p = 2, char _algo = 'b') 
		:k(_k), distp(_p), algo(_algo){}
	void		fit(items, label);
	label		predict(items);
	double		accuracy_score(label, label);

private:
	char		algo;	//使用何种算法计算K近邻？暴力搜索 or KD Tree
	int			distp;		//minkowski 距离的指数
	int			k;		//近邻数量
	//f_dist		dist_func;

private:
	void		_fit(items, label);
	void		_bulid_kdtree();
	label		_brute_predict();
	label		_kdtree_predict();
	label_t		_get_sample_label(point);
	label_t		_get_sample_label_kdtree(point);
	label_t		_get_most_common_label(label);
	double		_distance(point, point);
	double		_accuracy_score(label, label);
};

vector<vector<double>> read_csv_dou(unsigned, unsigned, const char*);
vector<int> read_csv_int(unsigned, const char*);

#endif