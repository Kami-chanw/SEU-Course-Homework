import pandas as pd
import matplotlib.pyplot as plt
from utils import *

# prepare
n_components = 9
data = {}

# load data
df_wine_white = pd.read_csv(r'data/winequality-white.csv', header=0,
                            sep=';').drop_duplicates().assign(category=0)
df_wine_red = pd.read_csv(r'data/winequality-red.csv', header=0,
                          sep=';').drop_duplicates().assign(category=1)
df_wine = pd.concat([df_wine_white, df_wine_red], axis=0)
data['Origin'] = {
    'X': df_wine.iloc[:, :-1].to_numpy(),
    'y': df_wine.iloc[:, -1].to_numpy()
}

# pre-process
for col in df_wine.columns[:-1]:
    Q1, Q3 = df_wine[col].quantile(0.25), df_wine[col].quantile(0.75)
    IQR = Q3 - Q1
    df_wine = df_wine[(Q1 - 1.5 * IQR <= df_wine[col])
                      & (df_wine[col] <= (Q3 + 1.5 * IQR))].reset_index(
                          drop=True)
df_wine.iloc[:, :-1] = df_wine.iloc[:, :-1].apply(lambda x:
                                                  (x - x.mean()) / x.std())
data["Origin(After cleansing)"] = {
    'X': df_wine.iloc[:, :-1].to_numpy(),
    'y': df_wine.iloc[:, -1].to_numpy()
}

X, y = data["Origin(After cleansing)"]['X'], data["Origin(After cleansing)"][
    'y']
# PCA
pca = PCA(n_components)
data['PCA'] = {'X': pca.fit_transform(X), 'y': y}

# LDA
lda = LDA()
data['LDA'] = {'X': lda.fit_transform(X, y), 'y': y}


def scatter_data(data):
    fig = plt.figure(figsize=(8, 6))
    for i, label in enumerate(data):
        y = data[label]['y']
        X = np.c_[data[label]['X'], y]
        ax = fig.add_subplot(2, 2, i + 1)
        ax.scatter(X[y == 0, 0], X[y == 0, 1], marker='x', label='white wine')
        ax.scatter(X[y == 1, 0], X[y == 1, 1], marker='o', label='red wine')
        ax.legend()
        ax.set_title(label)
    fig.tight_layout()
    plt.show()

scatter_data(data)

def plot_train_result(data):
    fig = plt.figure(figsize=(8, 6))
    fig.supxlabel("Epochs")
    fig.supylabel("Train loss")
    for i, label in enumerate(data):
        model = LogisticRegression()
        X_train, X_test, y_train, y_test = train_test_split(
            data[label]['X'], data[label]['y'])
        model.fit(X_train, y_train)
        print(
            f'train on {label}, accuracy : {accuracy(y_test, model.predict(X_test))}'
        )
        history = [x['train loss'] for x in model.history]
        ax = fig.add_subplot(2, 2, i + 1)
        ax.plot(range(1, len(history) + 1), history, label=label)
        ax.legend()
        ax.set_title(label)
    fig.tight_layout()
    plt.show()


plot_train_result(data)
