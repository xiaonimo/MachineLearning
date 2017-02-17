import matplotlib.pylab as plt
import pandas as pd

class LR:
    def load_data(self, file_path):
        _data = pd.read_csv(file_path, header=None)
        train_x = _data.iloc[0:_data.shape[0],0:2]
        train_y = _data.iloc[0:_data.shape[0],2]
        return train_x, train_y

    def print_data(self, x, y):
        y1 = y == 1
        d1 = x.loc[y1]
        plt.scatter(d1[[0]], d1[[1]], c='yellow')
        y0 = y == 0
        d0 = x.loc[y0]
        plt.scatter(d0[[0]], d0[[1]], c='red')
        plt.show()

    def fit(self, x, y, a):
        k = 1
        for _x, _y in x, y:
            k = k - a*_x

if __name__ == '__main__':
    l = LR()
    x,y = l.load_data('data.csv')
    print(x.shape, y.shape)
    l.print_data(x,y)

