import utils
import pandas as pd
from sklearn.decomposition import PCA
from sklearn.model_selection import train_test_split
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis
import matplotlib.pyplot as plt
import numpy as np

# prepare
n_components = 9
data = {}

df_wine_white = pd.read_csv(r'data/winequality-white.csv', header=0,
                            sep=';').drop_duplicates().assign(category=0)
df_wine_red = pd.read_csv(r'data/winequality-red.csv', header=0,
                          sep=';').drop_duplicates().assign(category=1)
df_wine = pd.concat([df_wine_white, df_wine_red], axis=0)

# pre-process
for col in df_wine.columns[:-1]:
    Q1, Q3 = df_wine[col].quantile(0.25), df_wine[col].quantile(0.75)
    IQR = Q3 - Q1
    df_wine = df_wine[(Q1 - 1.5 * IQR <= df_wine[col])
                      & (df_wine[col] <= (Q3 + 1.5 * IQR))].reset_index(
                          drop=True)
df_wine.iloc[:, :-1] = df_wine.iloc[:, :-1].apply(lambda x:
                                                  (x - x.mean()) / x.std())
X = df_wine.iloc[:, :-1].to_numpy()
y = df_wine.iloc[:, -1].to_numpy()

data['My PCA'] = utils.PCA(n_components=n_components).fit_transform(X)
data['My LDA'] = utils.LDA().fit_transform(X, y)
data['sklearn PCA'] = PCA(n_components=n_components).fit_transform(X)
data['sklearn LDA'] = LinearDiscriminantAnalysis().fit_transform(X, y)


def scatter_data(data):
    fig = plt.figure(figsize=(8, 6))
    for i, label in enumerate(data):
        X = np.c_[data[label], y]
        ax = fig.add_subplot(2, 2, i + 1)
        ax.scatter(X[y == 0, 0], X[y == 0, 1], marker='x', label='white wine')
        ax.scatter(X[y == 1, 0], X[y == 1, 1], marker='o', label='red wine')
        ax.legend()
        ax.set_title(label)
    fig.tight_layout()
    plt.show()


# scatter_data(data)


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
