# libraries
import matplotlib.pylab as plt
import pandas as pd
import time

class DBSCAN:
    # vars
    def __init__(self, data, eps=2, minPts=1):
        self.data = data
        self.eps1 = eps
        self.eps2 = eps**2
        self.minPts = minPts
        self.nData = data.shape[0]
        self.label = [-1]*self.nData
        self.clusters = []

    def ShowDistribution(self):
        color = ('red', 'green', 'blue', 'black', 'gray')
        i = 0
        for c in self.clusters:
            d = self.data.loc[c]
            plt.scatter(d[[0]], d[[1]], color=color[i])
            i = (i + 1) % 5
        plt.show()

    def cluster(self):
        # classify the point to Core, Border, Outline
        self._checkPts()
        # classify the core point
        self._classify()

    def _checkPts(self):
        for i in range(self.nData):
            p = self.data.loc[i]
            if self._isCorePoint(p):
                self.label[i] = 1
            elif self._isBorderPoint(p):
                self.label[i] = 2
            else:
                self.label[i] = 0

    def _classify(self):
        for i in range(self.nData):
            if self.label[i] == 0:
                continue
            cluster = [i]
            for index1 in cluster:
                self.label[index1] = 0
                for index2 in range(i + 1, self.nData, 1):
                    if self.label[index2] == 0:
                        continue
                    if self._isSameCluster(self.data.loc[index1], self.data.loc[index2]):
                        cluster.append(index2)
                        self.label[index2] = 0
            self.clusters.append(cluster)

    def _isSameCluster(self, p1, p2):
        if abs(p1[0] - p2[0]) > self.eps1 or abs(p1[1] - p2[1]) > self.eps1:
            return False
        return self._distance(p1, p2) < self.eps2

    def _isCorePoint(self, argP):
        cnt = 0
        for i in range(self.nData):
            if self.label[i] == 0:
                continue
            p = self.data.loc[i]
            if abs(p[0] - argP[0]) > self.eps1 or abs(p[1] - argP[1]) > self.eps1:
                continue
            if self._distance(argP, p) <= self.eps2:
                cnt += 1
                if cnt >= self.minPts:
                    return True
        return False

    def _isBorderPoint(self, argP):
        return False

    def _distance(self, p1, p2):
        return (p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2

#.txt file as default
def LoadData(path):
    data = pd.read_csv(path, sep=' ', header=None)
    # print(data.shape)
    return data
def show(data):
    plt.scatter(data[[0]], data[[1]], c='blue')
    plt.show()

# main code
if __name__ == '__main__':
    beg = time.time()
    data = LoadData('dataset3.txt')
    a = DBSCAN(data, 10, 3)
    a.cluster()
    lst = time.time()
    print(str(lst - beg) + 's')
    a.ShowDistribution()
    # show(LoadData('dataset3.txt'))
