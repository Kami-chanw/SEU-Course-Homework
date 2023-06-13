import numpy as np
import pandas as pd
from collections import Counter


def load_data():
    train_data = pd.read_csv('./data/train.csv')
    val_data = pd.read_csv('./data/val.csv')
    test_data = pd.read_csv('./data/test_data.csv')

    X_train = train_data.drop('label', axis=1).values
    y_train = train_data['label'].values

    X_val = val_data.drop('label', axis=1).values
    y_val = val_data['label'].values

    X_test = test_data.values

    X_all = np.concatenate((X_train, X_val, X_test), axis=0)
    mean = np.mean(X_all, axis=0)
    std = np.std(X_all, axis=0)
    X_train, X_val, X_test = (X_train - mean) / std, (X_val - mean) / std, (
        X_test - mean) / std

    return np.c_[X_train, y_train], np.c_[X_val, y_val], X_test


def cross_val_score(estimator, X, y, cv=5):
    scores = []
    fold_size = len(X) // cv

    for i in range(cv):
        start = i * fold_size
        end = start + fold_size

        X_val_fold = X[start:end]
        y_val_fold = y[start:end]

        X_train_fold = np.concatenate((X[:start], X[end:]), axis=0)
        y_train_fold = np.concatenate((y[:start], y[end:]), axis=0)

        estimator.fit(X_train_fold, y_train_fold)
        fold_score = estimator.score(X_val_fold, y_val_fold)
        scores.append(fold_score)

    return np.array(scores)


def find_best_k(gen_knn, X, y, k_values, cv=5):
    accuracies = []
    best_k, best_accuracy, best_knn = None, 0, None

    for k in k_values:
        knn = gen_knn(k)
        scores = cross_val_score(knn, X, y, cv=cv)
        accuracy = scores.mean()
        accuracies.append(accuracy)

        if accuracy > best_accuracy:
            best_k, best_accuracy, best_knn = k, accuracy, knn

    return best_k, best_knn, accuracies


def minkowski_distance(x, y, p):
    diff = np.abs(x - y)
    distance = np.power(np.sum(np.power(diff, p)), 1/p)
    return distance


def mahalanobis_distance(x, y, VI=None):
    diff = x - y
    if VI is None:
        return np.sqrt(diff @ np.linalg.inv(np.cov(x, y)) @ diff.T)
    return np.sqrt(diff @ np.linalg.inv(VI) @ diff.T)


class KNN:
    class KDNode:
        def __init__(self, point, label, left=None, right=None):
            self.point = point
            self.label = label
            self.left = left
            self.right = right

    def __init__(self,
                 n_neighbors,
                 p=2,
                 metric='minkowski',
                 metric_params=None):
        self.n_neighbors = n_neighbors
        self.distance_metric = metric
        self.n_features_in_ = None
        self.classes_ = None
        self.feature_names_in_ = None
        self.n_samples_fit_ = None
        self.kd_tree = None
        self.p = p
        self.metric_params = metric_params

    def fit(self, X, y):
        self.X_train = X
        self.y_train = y
        self.n_samples_fit_ = X.shape[0]
        self.n_features_in_ = X.shape[1]
        self.classes_ = np.unique(y)
        self.feature_names_in_ = [
            f'feature_{i+1}' for i in range(self.n_features_in_)
        ]
        self.kd_tree = self._build_kd_tree(X, y)

    def _build_kd_tree(self, X, y):
        if X.shape[0] == 0:
            return None

        axis = np.argmax(np.var(X, axis=0))
        sorted_indices = np.argsort(X[:, axis])
        sorted_X = X[sorted_indices]
        sorted_y = y[sorted_indices]
        median_index = len(X) // 2

        node = self.KDNode(sorted_X[median_index], sorted_y[median_index])
        node.left = self._build_kd_tree(sorted_X[:median_index],
                                        sorted_y[:median_index])
        node.right = self._build_kd_tree(sorted_X[median_index + 1:],
                                         sorted_y[median_index + 1:])

        return node

    def _get_distance(self, x1, x2):
        if self.distance_metric == 'minkowski':
            return minkowski_distance(x1, x2, self.p)
        elif self.distance_metric == 'mahalanobis':
            return mahalanobis_distance(x1, x2, self.metric_params['VI'])
        else:
            raise ValueError('Invalid distance metric.')

    def _knn_search(self, node, x, k_nearest, visited):
        if node is None:
            return

        visited.append(node)
        distance = self._get_distance(node.point, x)

        if len(k_nearest) < self.n_neighbors:
            k_nearest.append((node.point, node.label, distance))
            k_nearest.sort(key=lambda item: item[2], reverse=True)
        elif distance < k_nearest[0][2]:
            k_nearest[0] = (node.point, node.label, distance)
            k_nearest.sort(key=lambda item: item[2], reverse=True)

        axis = np.argmax(np.abs(x - node.point))
        if x[axis] < node.point[axis]:
            self._knn_search(node.left, x, k_nearest, visited)
            if abs(x[axis] - node.point[axis]) < k_nearest[0][2]:
                self._knn_search(node.right, x, k_nearest, visited)
        else:
            self._knn_search(node.right, x, k_nearest, visited)
            if abs(x[axis] - node.point[axis]) < k_nearest[0][2]:
                self._knn_search(node.left, x, k_nearest, visited)

    def _get_k_nearest_labels(self, k_nearest):
        k_nearest_labels = [item[1] for item in k_nearest]
        counter = Counter(k_nearest_labels)
        majority_label = counter.most_common(1)[0][0]
        return majority_label

    def predict(self, X):
        y_pred = []
        for x in X:
            visited = []
            k_nearest = []
            self._knn_search(self.kd_tree, x, k_nearest, visited)
            y_pred.append(self._get_k_nearest_labels(k_nearest))
        return np.array(y_pred)

    def score(self, X, y):
        return np.mean(self.predict(X) == y)