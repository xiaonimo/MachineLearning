# libraries
import matplotlib.pylab as plt
import matplotlib
import pandas as pd
import numpy as np


class db:
    #vars
    def __init__(self, path='dataset2.txt'):
        self.data_file_path = path
        self.data = None
        self.radius = 0
        self.num = 0
        self.len = 0
        self.distTable = []
        self.label=[]
    # functions
    def LoadData(self):
        data = pd.read_csv(self.data_file_path, sep=' ', header=None)
        #print(data.shape)
        self.data = data
        self.len = data.shape[0]
    def ShowDistribution(self, data):
        plt.scatter(data[[0]], data[[1]])
        plt.show()
    def DBscan(self, r, n):
        self.radius, self.num = r, n
        self.label = [0]*self.data.shape[0]

        #classify the point to Core, Border, Outline
        for i in range(len(self.label)):
            if self._isCorePoint(self.data.loc[i]):
                self.label[i] = 1
            elif self._isBorderPoint(self.data.loc[i]):
                self.label[i] = 2
            else:
                self.label[i] = 0
        #classify the core point
        label_core = [i for i in range(len(a.label)) if a.label[i] == 1]

    def _initDistTable(self):
        for i in range(self.len):
            dist = []
            for j in range(i+1, self.len, 1):
                dist.append(self._distance(self.data.loc[i], self.data.loc[j]))
            self.distTable.append(dist)
    def _isCorePoint(self, argP):
        cnt = 0
        for i in range(self.data.shape[0]):
            p = self.data.loc[i]
            if (abs(p[0] - argP[0]) >= self.radius and\
                            abs(p[1] - argP[1]) >= self.radius):
                continue
            if self._distance(argP, p) <= self.radius:
                cnt = cnt+1
        if cnt >= self.num:
            return True
        else:
            return False
    def _isBorderPoint(self, argP):
        return False
    def _distance(self, p1, p2):
        return ((p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2) ** 0.5

# main code
if __name__ == '__main__':
    a = db()
    a.LoadData()
    #a.ShowDistribution(a.data)
    a.DBscan(1, 2)


    label1 = [i for i in range(len(a.label)) if a.label[i] == 1]
    label2 = [i for i in range(len(a.label)) if a.label[i] == 0]
    corep = a.data.iloc[label1, :]
    borderp = a.data.iloc[label2, :]
    plt.scatter(corep[[0]], corep[[1]], color='red')
    plt.scatter(borderp[[0]], borderp[[1]], color='green')
    plt.show()


