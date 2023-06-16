import matplotlib.pyplot as plt
from sklearn.metrics import roc_curve, auc
import pandas as pd
from sklearn.svm import SVC
from sklearn.metrics import accuracy_score, f1_score, recall_score, precision_score

X_train = pd.read_csv('breast_cancer_Xtrain.csv', header=0).values
X_test = pd.read_csv('breast_cancer_Xtest.csv', header=0).values

y_train = pd.read_csv('breast_cancer_Ytrain.csv', header=0).values
y_test = pd.read_csv('breast_cancer_Ytest.csv', header=0).values

svm_linear_1 = SVC(kernel='linear', C=1)
svm_linear_1000 = SVC(kernel='linear', C=1000)
svm_poly_1 = SVC(kernel='poly', degree=2, C=1)
svm_poly_1000 = SVC(kernel='poly', degree=2, C=1000)

models = [
    "Linear SVM (C=1)", "Linear SVM (C=1000)",
    "Non-linear SVM (C=1, degree=2)", "Non-linear SVM (C=1000, degree=2)"
]
svms = [
    svm_linear_1, svm_linear_1000,
    svm_poly_1, svm_poly_1000
]


for model, svm in zip(models, svms):
    svm.fit(X_train, y_train.ravel())
    y_pred = svm.predict(X_test)
    accuracy = accuracy_score(y_test, y_pred)
    f1 = f1_score(y_test, y_pred)
    recall = recall_score(y_test, y_pred)
    precision = precision_score(y_test, y_pred)
    
    print(model)
    print("Accuracy:", accuracy)
    print("F1 Score:", f1)
    print("Recall:", recall)
    print("Precision:", precision)
    print()

plt.figure(figsize=(10, 8))
for model, svm in zip(models, svms):
    y_scores = svm.decision_function(X_test)
    fpr, tpr, _ = roc_curve(y_test, y_scores)
    roc_auc = auc(fpr, tpr)
    plt.plot(fpr, tpr, label=model + ' (AUC = %0.2f)' % roc_auc)

plt.plot([0, 1], [0, 1], 'k--')
plt.xlim([0, 1])
plt.ylim([0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver Operating Characteristic')
plt.legend(loc="lower right")
plt.show()
