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


def find_best_threshold(y_true, X, model):
    z = sigmoid(X @ model)
    thresholds = np.sort(np.unique(z))
    best_f1_score = 0
    best_threshold = 0
    for threshold in thresholds:
        y_pred = classify(z, threshold)
        precision, recall = calc_pr(y_pred, y_true)
        f1_score = 2 * precision * recall / (precision + recall)
        if f1_score > best_f1_score:
            best_f1_score = f1_score
            best_threshold = threshold
    return best_threshold


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
beta = np.linalg.inv(X_train.T @ X_train) @ X_train.T @ y_train

# predict & evaluate (question 2(1))
evaluate(predict(beta, X_val, 0.5), y_val)

# predict (question 2(2))
best_threshold = find_best_threshold(y_val, X_val, beta)
y_pred = predict(beta, X_test, best_threshold)
# predict output
np.savetxt("./ML4_programming/58121102_0.csv",
           y_pred,
           delimiter=',',
           header="class",
           fmt="%d")
