from models import Models
from preprocess import load_data
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--cv', type=int, default=3)
parser.add_argument('--jobs', type=int, default=1)
parser.add_argument('--verbose', type=int, default=3)
parser.add_argument('--clf', type=str, choices=['knn', 'svm'], default='svm')
parser.add_argument('--save', type=bool, default=True)
args = parser.parse_args()

if __name__ == '__main__':
    train_X, train_Y = load_data()
    test_X, test_idx = load_data('test')

    model = Models(args.clf, args.cv, args.verbose, args.jobs)
    model(train_X, train_Y)

    test_Y = model.predict(test_X, args.save)