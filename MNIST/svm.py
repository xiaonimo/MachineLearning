from sklearn import metrics
import numpy as np
import pandas as pd
import time
import csv
from sklearn.decomposition import PCA
from sklearn.preprocessing import PolynomialFeatures
from sklearn.svm import SVC

traindata_file = "../tf/data/TrainSamples.csv"
trainlabel_file = "../tf/data/TrainLabels.csv"
testdata_file = "../tf/data2/TestSamples.csv"
#testlabel_file = "../tf/data/TestLabels1.csv"

def pca_process(data):
	pca = PCA(n_components=35,whiten=True)
	pca.fit(data)
	data = pca.transform(data)
	return data

def _svm(data, label, k='rbf'):
	model = SVC(kernel=k, C=5)
	model.fit(data, label)
	return model

def read_data():
	train_data  = pd.read_csv(traindata_file, header=None)
	train_label = pd.read_csv(trainlabel_file, header=None)
	test_data   = pd.read_csv(testdata_file, header=None)
	#test_label  = pd.read_csv(testlabel_file, header= None)
	return train_data,train_label,test_data#,test_label
	
def write_csv(data, filename):
	with open(filename, 'w') as csvfile:
		writer = csv.writer(csvfile)
		for i in data:
			writer.writerows(str(i))

if __name__ == '__main__':
	#read data
	print("Reading train data and labels...")

	#train_data,train_label,test_data,test_label = read_data();
	train_data,train_label,test_data = read_data();	

	for i in range(1):
		print("PCA processing...")
		pca = PCA(n_components=60, whiten=True)
		pca.fit(train_data)
		ptrain_data = pca.transform(train_data)
		ptest_data = pca.transform(test_data)

		
		print("Test...")
		start_time = time.time();
		cur_model = _svm(ptrain_data, train_label.values.ravel())
		cur_predict = cur_model.predict(ptest_data)
		end_time = time.time()

		write_csv(cur_predict, 'result'+str(i)+'.csv')
		#print(i, str(end_time - start_time)+"s:", metrics.accuracy_score(test_label, cur_predict))
			
