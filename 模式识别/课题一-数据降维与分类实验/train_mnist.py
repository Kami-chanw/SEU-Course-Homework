from sklearn.datasets import load_digits
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegression
from sklearn.decomposition import PCA
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis, StandardScaler
import numpy as np
import utils
import matplotlib.pyplot as plt

# prepare
n_components = 0.9
data = {}
scaler = StandardScaler()

digits = load_digits()
X = scaler.fit_transform(digits.data)
y = digits.target

data['My PCA'] = utils.PCA(n_components=n_components).fit_transform(X)
data['My LDA'] = utils.LDA().fit_transform(X, y)
data['sklearn PCA'] = PCA(n_components=n_components).fit_transform(X)
data['sklearn LDA'] = LinearDiscriminantAnalysis().fit_transform(X, y)

def scatter_data(data):
    fig = plt.figure(figsize=(8, 6))
    for i, label in enumerate(data):
        X = np.c_[data[label], y]
        ax = fig.add_subplot(2, 2, i + 1)
        for num, marker in enumerate(['.', 'v', 'x', 's']):
            ax.scatter(X[y == num, 0], X[y == num, 1], marker=marker, label=f'{num}')
        ax.legend()
        ax.set_title(label)
    fig.tight_layout()
    plt.show()

scatter_data(data)

def plot_train_result(data):
    fig = plt.figure(figsize=(8, 4))
    fig.supxlabel("Epochs")
    fig.supylabel("Train loss")
    ax1 = fig.add_subplot(121)
    ax2 = fig.add_subplot(122)
    for label in data:
        model = utils.LogisticRegression()
        X_train, X_test, y_train, y_test = train_test_split(data[label], y)
        model.fit(X_train, y_train)
        print(
            f'train on {label}, accuracy : {utils.accuracy(y_test, model.predict(X_test))}'
        )
        history = [x['train loss'] for x in model.history]
        ax = ax1 if label.endswith('PCA') else ax2
        ax.plot(range(1, len(history) + 1), history, label=label)
    ax1.legend()
    ax2.legend()
    fig.tight_layout()
    plt.show()


plot_train_result(data)
