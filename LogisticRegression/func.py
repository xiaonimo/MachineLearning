import matplotlib.pylab as plt
import pandas as pd
from numpy import *
import time

class LR:
    def load_data(self, file_path):
        _data = pd.read_csv(file_path, header=None)
        train_x = _data.iloc[0:_data.shape[0],0:2]
        train_y = _data.iloc[0:_data.shape[0],2]
        return train_x, train_y

    def print_data(self, x, y, weights):
        y1 = y == 1
        d1 = x.loc[y1]
        plt.scatter(d1[[0]], d1[[1]], c='yellow')
        y0 = y == 0
        d0 = x.loc[y0]
        plt.scatter(d0[[0]], d0[[1]], c='red')

        linex = arange(-6, 6, 0.1)
        liney = (-weights[0] - weights[1]*linex)/weights[2]
        plt.plot(linex, liney.transpose())
        plt.title('LR(gd)')
        plt.show()

    def _sigmoid(self, x):
        return 1.0/(1.0+exp(-x))

    def fit(self, x, y, alpha, itr):
        x = c_[ones(x.shape[0]), x]
        x = mat(x)
        y = mat(y).transpose()
        _weights = ones((x.shape[1], 1))
        for i in range(itr):
            h = self._sigmoid(x*_weights)
            err = y - h
            _weights = _weights + alpha*x.transpose()*err
        return _weights

if __name__ == '__main__':
    l = LR()
    x,y = l.load_data('data.csv')
    w = l.fit(x, y, 0.01, 600)
    l.print_data(x, y, w)

