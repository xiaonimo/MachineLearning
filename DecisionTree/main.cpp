#include <iostream>
#include <stdio.h>
#include "id3.h"

using namespace std;

int main() {
	//������ɢֵ��������
	//vector<int> v{5,5,5,5,3};
	items RawData = getData(625, 5);

	//���Ļ������ԣ�
	id3 a(RawData);
	a.bulidTree();
	//a.printTree();

	//���֦����
	a.post_pruning();
	a.printTree();

	//classfy���ܲ���
	/*items test_data{ { 1, 1, 1, 1 }, { 1, 1, 1, 2 } };
	point test_label = a.classify(test_data);
	for (auto n : test_label) cout << n;*/

	//holdout��������
	//cout << holdout(getData(625, 5), v);

	//��֤K�۽�����֤
	//KcrossValidation(RawData, 10);

	//��֤bootStrap
	//cout << bootStrap(RawData,5);

	


	freopen("CON", "r", stdin);
	system("pause");
	return 0;
}

/*
���������е����⣺
1.���ຯ����ѭ������Ϊ���ݼ��������ԣ��ڷ����ʱ��ûӴ��Ӧ�����
2.KcrossValidation�ڲ��Լ�ȡ�ڶ����ֵ�ʱ����split�����г���vectorԽ��bug
  �����±궼����������Χ�ڣ����ݼ�Ҳ�������ġ�Ϊ����
3.
 */