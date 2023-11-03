import numpy as np
import copy
import pandas as pd
import random
from sklearn.metrics import accuracy_score


class SVM:
    def __init__(self, C=1.0, kernel='linear', toler=0.001, max_iter=40):
        self.C = C
        self.kernel = kernel
        self.tol = toler
        self.max_iter = max_iter
        self.b = None
        self.alpha = None
        self.w = None
        self.support_vectors_ = None
        self.dual_coef_ = None
        self.intercept_ = None
        self.support_ =None

    def _select_j(self, i, m):
        j = i
        while j == i:
            j = int(random.uniform(0, m))
        return j

    def _clip_alpha(self, alpha, H, L):
        if alpha > H:
            alpha = H
        if L > alpha:
            alpha = L
        return alpha

    def _kernel(self, X1, X2):
        if self.kernel == 'linear':
            return X1 @ X2.T if X2 is not None else X1 @ X1.T

    def fit(self, X, y):
        self.n_samples, self.n_features = np.shape(X)
        self.b = 0
        self.alpha = np.zeros((self.n_samples, 1))
        iter_num = 0
        while iter_num < self.max_iter:
            alpha_changed = 0
            for i in range(self.n_samples):
                j = self._select_j(i, self.n_samples)
                x1, x2 = X[i, :], X[j, :]
                # calculate E1
                gx1 = np.mean(self.alpha * y * self._kernel(X, x1)) + self.b
                E1 = gx1 - float(y[i])
                if (y[i] * E1 < -self.tol
                        and self.alpha[i] < self.C) or (y[i] * E1 > self.tol
                                                        and self.alpha[i] > 0):

                    # calculate E2
                    gx2 = np.mean(
                        self.alpha * y * self._kernel(X, x2)) + self.b
                    E2 = gx2 - float(y[j])
                    # create α1_old and α2_old
                    alpha1_old = copy.deepcopy(self.alpha[i])
                    alpha2_old = copy.deepcopy(self.alpha[j])
                    # calculate H and L
                    if y[i] != y[j]:
                        L = max(0, alpha2_old - alpha1_old)
                        H = min(self.C, self.C + alpha2_old - alpha1_old)
                    else:
                        L = max(0, alpha2_old + alpha1_old - self.C)
                        H = min(self.C, alpha2_old + alpha1_old)
                    # get eta
                    eta = self._kernel(x1, x1) + self._kernel(
                        x2, x2) - 2 * self._kernel(x1, x2)
                    # get α2_new_unt
                    self.alpha[j] += y[j] * (E1 - E2) / eta
                    # get α2_new
                    self.alpha[j] = self._clip_alpha(self.alpha[j], H, L)
                    # get α1_new
                    self.alpha[i] += y[j] * y[i] * (alpha2_old - self.alpha[j])

                    # get b1_new and b2_new
                    b1 = self.b - E1 - y[i] * self._kernel(x1, x1) * (
                        self.alpha[i] - alpha1_old) - y[j] * self._kernel(
                            x2, x1) * (self.alpha[j] - alpha2_old)
                    b2 = self.b - E2 - y[i] * self._kernel(x1, x2) * (
                        self.alpha[i] - alpha1_old) - y[j] * self._kernel(
                            x2, x2) * (self.alpha[j] - alpha2_old)
                    if 0 < self.alpha[i] < self.C or 0 < self.alpha[j] < self.C:
                        self.b = b2
                    else:
                        self.b = (b1 + b2) / 2
                    alpha_changed += 1
            if alpha_changed == 0:
                iter_num += 1
            else:
                iter_num = 0
        self.w = np.mean(self.alpha * y * X, axis=0)

        # Store support vectors, dual coefficients, and intercept
        self.support_ = np.where(self.alpha > 0)[0]
        self.support_vectors_ = X[self.support_]
        self.dual_coef_ = self.alpha * y
        self.intercept_ = self.b

    def predict(self, X):
        return np.sign(self.decision_function(X))
    
    def decision_function(self, X):
        return np.dot(X, self.w) + self.b


X_train = pd.read_csv('breast_cancer_Xtrain.csv', header=0).values
X_test = pd.read_csv('breast_cancer_Xtest.csv', header=0).values

y_train = pd.read_csv('breast_cancer_Ytrain.csv', header=0).values
y_test = pd.read_csv('breast_cancer_Ytest.csv', header=0).values

classifier = SVM(C=2)

classifier.fit(X_train, y_train)

y_pred = classifier.predict(X_test)

accuracy = accuracy_score(y_test, y_pred)
print("准确率：", accuracy)
