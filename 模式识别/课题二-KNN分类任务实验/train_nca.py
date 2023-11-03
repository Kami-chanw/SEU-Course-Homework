import numpy as np
import utils
import matplotlib.pyplot as plt
import csv

class NCA:
    def __init__(self, n_components=None, learning_rate=0.01, max_iter=500):
        self.n_components = n_components
        self.learning_rate = learning_rate
        self.max_iter = max_iter
        self.components_ = None

    def fit(self, X, y):
        n_samples, n_features = X.shape
        self.n_samples = n_samples
        self.n_features = n_features

        # parametric matrix
        self.components_ = 0.1 * np.random.standard_normal(
            size=(self.n_features, self.n_components))

        # training using gradient descent
        for step in range(self.max_iter):

            # to low dimension
            XAt = X @ self.components_

            # distance matrix --> proba_matrix
            sum_row = np.sum(XAt**2, axis=1)
            pij_mat = sum_row + np.reshape(sum_row, (-1, 1)) - 2 * XAt @ XAt.T
            pij_mat = np.exp(-pij_mat)
            np.fill_diagonal(pij_mat, 0.0)
            pij_mat = pij_mat / np.sum(pij_mat, axis=1)[:, None]

            # mask where maskij = True if y[i] == y[j]
            mask = (y == y[:, None])

            # mask array
            pij_mat_mask = pij_mat * mask

            # pi = \sum_{j \in \Omega_i} pij
            pi_arr = np.sum(pij_mat_mask, axis=1)

            # target
            self.target = np.sum(pi_arr)

            # gradients
            weighted_pij = pij_mat_mask - pij_mat * pi_arr[:, None]
            weighted_pij_sum = weighted_pij + weighted_pij.T
            np.fill_diagonal(weighted_pij_sum, -weighted_pij.sum(axis=0))
            gradients = 2 * (XAt.T @ weighted_pij_sum @ X).T

            # update
            self.components_ += self.learning_rate * gradients
        return self

    def transform(self, X):
        return np.dot(X, self.components_)

    def fit_transform(self, X, y):
        return self.fit(X, y).transform(X)


train_data, val_data, X_test = utils.load_data()
X_train, y_train = train_data[:, :-1], train_data[:, -1]
X_val, y_val = val_data[:, :-1], val_data[:, -1]
nca = NCA(n_components=2)
X_train = nca.fit_transform(X_train, y_train)
X_test = nca.transform(X_test)
X_val = nca.transform(X_val)

best_k, best_knn, acc = utils.find_best_k(
    lambda k: utils.KNN(k, metric='mahalanobis',
                                   metric_params={'VI': np.cov(X_train.T)}), X_train, y_train,
    [x + 1 for x in range(10)])

plt.plot(range(1, 11), acc)
plt.xlabel('k value')
plt.ylabel('Accuracy')
plt.show()
y_test_pred = best_knn.predict(X_test)
print("Validation Accuracy (Euclidean Distance):",
      best_knn.score(X_val, y_val))
print('The best k', best_k)

with open('task2_test_prediction.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['label'])
    writer.writerows(y_test_pred.reshape(-1, 1).astype(int))