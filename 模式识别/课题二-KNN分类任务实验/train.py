import matplotlib.pyplot as plt
from utils import *
import csv
train_data, val_data, X_test = load_data()

X_train, y_train = train_data[:, :-1], train_data[:, -1]
X_val, y_val = val_data[:, :-1], val_data[:, -1]

best_k, best_knn, acc = find_best_k(lambda k: KNN(k), X_train, y_train,
                                    [x + 1 for x in range(10)])

plt.plot(range(1, 11), acc)
plt.xlabel('k value')
plt.ylabel('Accuracy')
plt.show()

y_test_pred = best_knn.predict(X_test)
print("Validation Accuracy (Euclidean Distance):",
      best_knn.score(X_val, y_val))
print('The best k', best_k)

with open('task1_test_prediction.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['label'])
    writer.writerows(y_test_pred.reshape(-1, 1).astype(int))