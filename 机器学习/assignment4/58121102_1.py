import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import warnings


def classify(z, threshold):
    y_pred = z.copy()
    y_pred[y_pred < threshold] = 0
    y_pred[y_pred > threshold] = 1
    return y_pred


def sigmoid(z):
    z[z > 0] = 1 / (1 + np.exp(-z[z > 0]))
    z[z < 0] = np.exp(z[z < 0]) / (1 + np.exp(z[z < 0]))
    return z


def predict(model, X, theta):
    return classify(sigmoid(X @ model), theta)


def calc_pr(y_pred, y_true):
    TP, FP, FN = [
        e.sum() for e in [
            y_pred[y_true == 1] == 1, y_pred[y_true == 0] == 1, y_pred[
                y_true == 1] == 0
        ]
    ]
    return (TP / (TP + FP), TP / (TP + FN))


def evaluate(y_pred, y_true):
    p, r = calc_pr(y_pred, y_true)
    print(f"accuracy:  {(y_true==y_pred).mean()}")
    print(f"precision: {p}")
    print(f"recall:    {r}")


warnings.filterwarnings('ignore', category=RuntimeWarning)
# data reading
X_train = pd.read_csv('./ML4_programming/train_features.csv').assign(
    argumented=1).to_numpy()
y_train = pd.read_csv('./ML4_programming/train_target.csv').to_numpy()
X_val = pd.read_csv('./ML4_programming/val_features.csv').assign(
    argumented=1).to_numpy()
y_val = pd.read_csv('./ML4_programming/val_target.csv').to_numpy()
X_test = pd.read_csv('./ML4_programming/test_feature.csv').assign(
    argumented=1).to_numpy()

# model training
epoch = int(800)
beta = np.full(shape=(X_train.shape[1], 1),
               fill_value=0.114151919810,
               dtype=float)
for i in range(epoch):
    z = X_train @ beta
    z[z > 0] = 1 / (1 + np.exp(-z[z > 0]))
    z[z < 0] = np.exp(z[z < 0]) / (1 + np.exp(z[z < 0]))
    df = X_train.T @ (z - y_train)
    beta -= df * 0.05

# predict & evaluate (question 2(3))
evaluate(predict(beta, X_val, 0.5), y_val)

# predict (question 2(4))
y_pred = predict(beta, X_test, 0.5)
# predict output
np.savetxt("./ML4_programming/58121102_1.csv",
           y_pred,
           delimiter=',',
           header="class",
           fmt="%d")