import numpy as np

BASE_PATH  = './Kannada-MNIST'
TRAIN_PATH = f'{BASE_PATH}/train.csv'
TEST_PATH  = f'{BASE_PATH}/test.csv'
DATA = {
    'train': TRAIN_PATH,
    'test' : TEST_PATH
}

def load_data(dtype='train', normalized=True):
    assert DATA.get(dtype, None), "Please check the data content type"
    data = np.loadtxt(DATA[dtype], delimiter=',', skiprows=1)

    label = data[:, 0]
    data = data[:, 1:]

    if normalized:
        data = data / 255.
    
    return (data, label)

if __name__ == '__main__':
    X, y = load_data()

    print(X.shape)
    print(y.shape)