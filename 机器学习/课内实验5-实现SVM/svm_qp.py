import numpy as np
import pandas as pd
from scipy.optimize import minimize
from sklearn.metrics import accuracy_score


class SVM:
    def __init__(self, C=1.0):
        self.C = C
        self.alpha = None
        self.b = None

    def fit(self, X, y):
        self.n_samples, self.n_features = X.shape

        constraints = [{
            'type': 'eq',
            'fun': lambda alpha: alpha @ y
        }, {
            'type': 'ineq',
            'fun': lambda alpha: alpha
        }, {
            'type': 'ineq',
            'fun': lambda alpha: self.C - alpha
        }]
        alpha_init = np.zeros(self.n_samples)
        result = minimize(self._objective_function,
                          alpha_init,
                          args=(X, y),
                          constraints=constraints)
        self.alpha = result.x[:, np.newaxis]

        self.b = np.mean(y - np.outer(self.alpha, y) * (X @ X.T))
        self.w = np.mean(self.alpha * y * X, axis=0)

    def _objective_function(self, alpha, X, y):
        return 0.5 * np.sum(
            np.outer(alpha, alpha) * np.outer(y, y) *
            (X @ X.T)) - np.sum(alpha)

    def predict(self, X):
        return np.sign(self.decision_function(X))
    
    def decision_function(self, X):
        return np.dot(X, self.w) + self.b

X_train = pd.read_csv('breast_cancer_Xtrain.csv', header=0).values
X_test = pd.read_csv('breast_cancer_Xtest.csv', header=0).values

y_train = pd.read_csv('breast_cancer_Ytrain.csv', header=0).values
y_test = pd.read_csv('breast_cancer_Ytest.csv', header=0).values

svm = SVM(0)

svm.fit(X_train, y_train)

y_pred = svm.predict(X_test)

print(accuracy_score(y_test, y_pred))
