import numpy as np
from sklearn.svm import SVC
from sklearn.model_selection import GridSearchCV
from sklearn.neighbors import KNeighborsClassifier

class Models(object):

    def __init__(self, clf, cv, verbose, n_jobs):
        classifiers = {
            'knn': KNeighborsClassifier(),
            'svm': SVC()
        }
        self.clf_type = clf
        self.clf = GridSearchCV(classifiers[clf], self.hyperparams,
                                cv=cv, verbose=verbose, n_jobs=n_jobs)
    

    def __call__(self, x, y):
        self.clf.fit(x, y)
        grid_scores = self.clf.grid_scores_
        best_params = self.clf.best_params_
        best_score = self.clf.best_score_
        print(f'{grid_scores}, {best_params}, {best_score}')

    def predict(self, x, flag=True):
        preds = self.clf.predict(x)

        if flag:
            rows = [['id', 'label']]
            for i, l in enumerate(preds):
                rows.append([i, int(l)])
            np.savetxt(f'{self.clf_type}-pred.csv', rows, fmt="%s,%s")

    @property
    def hyperparams(self):
        params = {
            'svm': {
                'C': [i/10. for i in range(10, 11)],
                'kernel': ['rbf'],
            },
            'knn': {
                'n_neighbors': [i for i in range(3, 10)],
                'weights': ['uniform', 'distance'],
                'p': [i for i in range(1, 7)]
            }
        }
        return params[self.clf_type]
    
