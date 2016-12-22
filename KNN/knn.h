#include <iostream>
#include <vector>

using namespace std;

class KNN{
	typedef double 			point_t;
	typedef vector<point_t> 	point;
	typedef vector<point>		items;
	typedef int			label_t;
	typedef vector<label_t> 	label;
	typedef double(*f_dist)(point, point);

public:
	items		train_data;
	items		test_data;
	label		train_label;
	label		test_label;

public:
	KNN(int _k = 5, int _p = 2, char _algo = 'b', f_dist _f = nullptr) 
		:k(_k), p(_p), algo(_algo), dist_func(_f){}
	void		fit(items, label);
	label		predict(items);
	double		accuracy_score(label, label);

private:
	char		algo;		//使用何种算法计算K近邻？暴力搜索 or KD Tree
	int		p;		//minkowski 距离的指数
	int		k;		//近邻数量
	f_dist		dist_func;

private:
	void		_fit(items, label);
	void		_bulid_kdtree();
	label		_brute_predict();
	label		_kdtree_predict();
	label_t		_get_sample_label(point);
	label_t		_get_sample_label_kdtree(point);
	double		_distance(point, point);
	double		_accuracy_score(label, label);
};
