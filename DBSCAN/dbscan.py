# libraries
import matplotlib.pylab as plt
import pandas as pd
import time


class db:
    # vars
    def __init__(self, path='dataset2.txt'):
        self.data_file_path = path
        self.data = None
        self.radius = 0
        self.num = 0
        self.len = 0
        self.label = []
        self.clusters = []

    def LoadData(self):
        data = pd.read_csv(self.data_file_path, sep=' ', header=None)
        # print(data.shape)
        self.data = data
        self.len = data.shape[0]

    def ShowDistribution(self):
        color = ('red', 'green', 'blue', 'black', 'gray')
        i = 0
        for c in self.clusters:
            d = self.data.loc[c]
            plt.scatter(d[[0]], d[[1]], color=color[i])
            i = (i + 1) % 5
        plt.show()

    def DBscan(self, r, n):
        self.radius, self.num, self.label = r, n, [0] * self.len
        # classify the point to Core, Border, Outline
        for i in range(self.len):
            p = self.data.loc[i]
            if self._isCorePoint(p):
                self.label[i] = 1
            elif self._isBorderPoint(p):
                self.label[i] = 2
            else:
                pass
        # classify the core point
        self._classify()

    def _classify(self):
        label_core = [i for i in range(self.len) if a.label[i] == 1]
        for i in range(self.len):
            if self.label[i] == 0:
                continue
            cluster = [i]
            for index1 in cluster:
                self.label[index1] = 0
                for index2 in range(i + 1, self.len, 1):
                    if self.label[index2] == 0:
                        continue
                    if self._isSameCluster(self.data.loc[index1], self.data.loc[index2]):
                        cluster.append(index2)
                        self.label[index2] = 0
            self.clusters.append(cluster)

    def _isSameCluster(self, p1, p2):
        if (abs(p1[0] - p2[0]) > self.radius or abs(p1[1] - p2[1]) > self.radius):
            return False
        return self._distance(p1, p2) < self.radius

    def _isCorePoint(self, argP):
        cnt = 0
        for i in range(self.len):
            p = self.data.loc[i]
            if (abs(p[0] - argP[0]) > self.radius or abs(p[1] - argP[1]) > self.radius):
                continue
            if self._distance(argP, p) <= self.radius:
                cnt = cnt + 1
                if cnt >= self.num:
                    return True
        return False

    def _isBorderPoint(self, argP):
        return False

    def _distance(self, p1, p2):
        return ((p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2) ** 0.5


# main code
if __name__ == '__main__':
    beg = time.time()

    a = db()
    a.LoadData()
    a.DBscan(2, 1)
    a.ShowDistribution()

    lst = time.time()
    print(str(lst - beg) + 's')
