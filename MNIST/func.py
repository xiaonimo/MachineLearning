import matplotlib.pyplot as plt
import matplotlib.cm as cm
import pandas as pd
import csv


def read_data(path):
    train = pd.read_csv(path, header=None, low_memory=False)
    # test = pd.read_csv(k_test_file, header=None, low_memory=False)
    # train_data = train.iloc[1:train.shape[0], 1:train.shape[1]]
    # train_label = train.iloc[1:train.shape[0], 0]
    # test_data = test.iloc[1:test.shape[0], 0:train.shape[1]]
    index = 10001
    test_data = train.iloc[1:index, 1:train.shape[1]]
    train_data = train.iloc[index:train.shape[0], 1:train.shape[1]]
    test_label = train.iloc[1:index, 0]
    train_label = train.iloc[index:train.shape[0], 0]

    return train_data, train_label.values.ravel(), test_data, test_label.values.ravel()


def write_csv(data, filename):
    with open(filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['ImageId','Label'])
        i = 1
        for d in data:
            writer.writerow([str(i), str(d)])
            i = i+1


def display(vec, width=28, height=28):
    image = vec.reshape(width, height)
    # print(image.shape)
    plt.axis('off')
    plt.imshow(image, cmap=cm.binary)
    plt.show()


if __name__ == '__main__':
    train_data = pd.read_csv('train.csv', low_memory=False)
    images = train_data.iloc[:,1:].values
    images[images<50] = 0
    images[images>0] = 255
    # images = images.astype(np.float)
    # images = np.multiply(images, 1.0/255.0)

    for i in range(train_data.shape[0]):
        input("next")
        image_vec = images[i+1]
        print(i, image_vec.shape)
        display(image_vec)
