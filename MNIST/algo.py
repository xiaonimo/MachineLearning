from sklearn.decomposition import PCA
from sklearn.svm import SVC


def pre_process(data, pixel=200):
    val = data.iloc[:,:].values
    val[val<pixel] = 0.0
    #data = data.astype(np.float64)
    #print(data.shape)
    return val

def pca_process(train_data, test_data, component=40):
    pca = PCA(n_components=component, whiten=True)
    pca.fit(train_data)
    ptrain_data = pca.transform(train_data)
    ptest_data = pca.transform(test_data)
    return ptrain_data,ptest_data


def svm_process(data, label, c=5):
    model = SVC(C=c)
    model.fit(data, label)
    return model
