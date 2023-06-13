from sklearn.datasets import load_breast_cancer
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.neighbors import KNeighborsClassifier
import matplotlib.pyplot as plt
from utils import *

X, y = load_breast_cancer(return_X_y=True)

X_train, X_test, y_train, y_test = train_test_split(X,
                                                    y,
                                                    test_size=0.2,
                                                    random_state=42)

scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)
X_test = scaler.transform(X_test)

result = {}
k_values = [x for x in range(1, 11)]
result['sklearn KNN with Euclidean'] = find_best_k(
    lambda k: KNeighborsClassifier(n_neighbors=k, p=2), X_train, y_train,
    k_values)
result['sklearn KNN with Mahalanobis'] = find_best_k(
    lambda k: KNeighborsClassifier(n_neighbors=k,
                                   metric='mahalanobis',
                                   metric_params={'VI': np.cov(X_train.T)}),
    X_train, y_train, k_values)
result['My KNN with Euclidean'] = find_best_k(lambda k: KNN(n_neighbors=k),
                                              X_train, y_train, k_values)
result['My KNN with Mahalanobis'] = find_best_k(
    lambda k: KNN(n_neighbors=k, metric='mahalanobis',
                                   metric_params={'VI': np.cov(X_train.T)}), X_train, y_train,
    k_values)

for label in result:
    best_k, best_knn, acc = result[label]
    print(f"Test Accuracy of {label}: {best_knn.score(X_test, y_test)}")
    print(f'The best k of {label}: {best_k}')
    plt.plot(range(1, 11), acc, label=label)
plt.legend()
plt.xlabel('k value')
plt.ylabel('Accuracy')
plt.show()


