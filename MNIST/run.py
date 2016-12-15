import time
from sklearn import metrics
from algo import *
from func import *

k_train_file = 'train.csv'
k_test_file = 'test.csv'

# train_data, train_label, test_data= read_data()
train_data, train_label, test_data, test_label = read_data(k_train_file)
print(train_data.shape)
print(train_label.shape)
print(test_data.shape)

# train_data = pre_process(train_data)
# test_data = pre_process(test_data)

for component in [34, 35, 36, 37, 39, 40, 42, 43, 44, 45]:
    t1 = time.time()
    ptrain_data, ptest_data = pca_process(train_data, test_data, component)

    t2 = time.time()
    cur_model = svm_process(ptrain_data, train_label, c=5)

    t3 = time.time()
    cur_predict = cur_model.predict(ptest_data)
    t4 = time.time()

    write_csv(cur_predict, 'result' + '.csv')
    print('PCA time:', str(t2 - t1) + 's')
    print('Learning time:', str(t3 - t2) + 's')
    print('Classify time:', str(t4 - t3) + 's')
    print('n_components:', component)
    print('C:', 5)
    print('Accuracy:', metrics.accuracy_score(test_label, cur_predict))
    print('----------------------------')
