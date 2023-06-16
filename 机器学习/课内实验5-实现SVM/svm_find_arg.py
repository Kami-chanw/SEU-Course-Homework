import matplotlib.pyplot as plt
from sklearn.metrics import roc_curve, auc, accuracy_score, precision_score, recall_score, f1_score
import numpy as np
from sklearn.svm import SVC
import pandas as pd

X_train = pd.read_csv('breast_cancer_Xtrain.csv', header=0).values
X_test = pd.read_csv('breast_cancer_Xtest.csv', header=0).values

y_train = pd.read_csv('breast_cancer_Ytrain.csv', header=0).values.ravel()
y_test = pd.read_csv('breast_cancer_Ytest.csv', header=0).values.ravel()

param_grid = {
    'C': np.linspace(0.1,3, num=10),
    'kernel': ['linear', 'poly', 'rbf']
}
accuracy_scores = []
precision_scores = []
recall_scores = []
f1_scores = []

for c in param_grid['C']:
    accuracy_kernel = []
    precision_kernel = []
    recall_kernel = []
    f1_kernel = []
    
    for kernel in param_grid['kernel']:
        svm = SVC(C=c, kernel=kernel)
        svm.fit(X_train, y_train)
        y_pred = svm.predict(X_test)
        
        accuracy = accuracy_score(y_test, y_pred)
        precision = precision_score(y_test, y_pred)
        recall = recall_score(y_test, y_pred)
        f1 = f1_score(y_test, y_pred)
        
        accuracy_kernel.append(accuracy)
        precision_kernel.append(precision)
        recall_kernel.append(recall)
        f1_kernel.append(f1)
    
    accuracy_scores.append(accuracy_kernel)
    precision_scores.append(precision_kernel)
    recall_scores.append(recall_kernel)
    f1_scores.append(f1_kernel)

accuracy_scores = np.array(accuracy_scores)
precision_scores = np.array(precision_scores)
recall_scores = np.array(recall_scores)
f1_scores = np.array(f1_scores)

plt.figure(figsize=(8, 6))

plt.subplot(2, 2, 1)
for i, kernel in enumerate(param_grid['kernel']):
    plt.plot(param_grid['C'], accuracy_scores[:, i], label=kernel)

plt.xlabel('C')
plt.ylabel('Accuracy')
plt.title('Accuracy vs. C for Different Kernels')
plt.legend()

plt.subplot(2, 2, 2)
for i, kernel in enumerate(param_grid['kernel']):
    plt.plot(param_grid['C'], precision_scores[:, i], label=kernel)

plt.xlabel('C')
plt.ylabel('Precision')
plt.title('Precision vs. C for Different Kernels')
plt.legend()

plt.subplot(2, 2, 3)
for i, kernel in enumerate(param_grid['kernel']):
    plt.plot(param_grid['C'], recall_scores[:, i], label=kernel)

plt.xlabel('C')
plt.ylabel('Recall')
plt.title('Recall vs. C for Different Kernels')
plt.legend()

plt.subplot(2, 2, 4)
for i, kernel in enumerate(param_grid['kernel']):
    plt.plot(param_grid['C'], f1_scores[:, i], label=kernel)

plt.xlabel('C')
plt.ylabel('F1 Score')
plt.title('F1 Score vs. C for Different Kernels')
plt.legend()

plt.tight_layout()
plt.show()
