import pandas as pd
import numpy as np
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import train_test_split
from sklearn.metrics import r2_score, mean_squared_error
from sklearn.decomposition import PCA
from sklearn.preprocessing import MinMaxScaler
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
from statsmodels.stats.outliers_influence import variance_inflation_factor

# elegant prepocess
df = pd.read_csv('song_data.csv').drop('song_name', axis=1).drop_duplicates()
attrs = [cols for cols in df.columns if cols not in ['song_popularity']]
category_attrs = ['time_signature', 'key', 'audio_mode']
numeric_attrs = [
    'danceability', 'energy', 'speechiness', 'audio_valence', 'liveness',
    'acousticness', 'instrumentalness', 'loudness', 'song_duration_ms', 'tempo'
]
df = pd.get_dummies(df, columns=category_attrs[:2])

for attr in numeric_attrs:
    Q1, Q3 = df[attr].quantile(0.25), df[attr].quantile(0.75)
    IQR = Q3 - Q1
    df = df[(Q1 - 1.5 * IQR <= df[attr])
            & (df[attr] <= (Q3 + 1.5 * IQR))].reset_index(drop=True)

scaler = MinMaxScaler()
df[numeric_attrs] = scaler.fit_transform(df[numeric_attrs])

X = df.drop('song_popularity', axis=1)
y = df['song_popularity']
train_X, test_X, train_y, test_y = train_test_split(X,
                                                    y,
                                                    train_size=0.8,
                                                    test_size=0.2,
                                                    random_state=100)

# feature extraction

# check for multicollinearity
vif = pd.DataFrame()
vif["variables"] = X.columns
vif["VIF"] = [variance_inflation_factor(X, i) for i in range(X.shape[1])]
if np.any(vif["VIF"]> 5):
    # PCA exploration
    pca = PCA().fit(train_X)
    # fig, ax = plt.subplots()
    x_values = range(1, pca.n_components_ + 1)
    # plt.bar(x_values, pca.explained_variance_ratio_, label='Explained Variance')
    # ax.plot(x_values,
    #         np.cumsum(pca.explained_variance_ratio_),
    #         label='Cumulative Explained Variance',
    #         color='red')
    threshold = 0.95
    # plt.plot([0, pca.n_components_ + 1], [threshold, threshold], "g--")
    f = interp1d(x_values, np.cumsum(pca.explained_variance_ratio_), kind="linear")
    cev_x = np.linspace(1, pca.n_components_, 1000)
    cev_y = f(cev_x)  # Cumulative Explained Variance
    idx = np.argwhere(np.isclose(cev_y, [threshold] * len(cev_y),
                                atol=1e-3)).flatten()
    idx = idx[int(len(idx) / 2)]
    # ax.scatter(cev_x[idx], cev_y[idx])
    # ax.text(cev_x[idx], cev_y[idx]-0.05, f"({round(cev_x[idx])},{round(cev_y[idx], 2)})", fontsize=12)
    # ax.set_xlabel('Principal Component')
    # ax.set_yticks([0,0.2,0.4,0.6,0.8,1.0,threshold])
    # ax.set_xlim((0, pca.n_components_ + 1))
    # plt.legend()
    # plt.grid()
    # plt.show()

    pca = PCA(n_components=int(cev_x[idx]))
    train_X = pd.DataFrame(pca.fit_transform(train_X))
    test_X = pd.DataFrame(pca.fit_transform(test_X))

LR = LinearRegression().fit(train_X, train_y)
train_pred = LR.predict(train_X)
test_pred = LR.predict(test_X)

print('coeffecients', LR.coef_)
print('intercepts', LR.intercept_)

f, axs = plt.subplots(2, 3, sharey=True)
f.set_figwidth(15)
f.set_figheight(8)
rc = np.random.choice(range(0, pca.n_components_), len(axs[0]), replace=False)
for i, col in enumerate(rc):
    axs[0][i].set_title(f'Training set {i+1}#')
    axs[0][i].scatter(train_X[col], train_y, label='real')
    axs[0][i].scatter(train_X[col], train_pred, label='pred')
    axs[0][i].legend()
for i, col in enumerate(rc):
    axs[1][i].set_title(f'Test set {i+1}#')
    axs[1][i].scatter(test_X[col], test_y, label='real')
    axs[1][i].scatter(test_X[col], test_pred, label='pred')
    axs[1][i].legend()

plt.show()

print('MSE of Training set', mean_squared_error(train_y, train_pred))
print('R2-Score of Testing set', r2_score(train_y, train_pred))
print('MSE of Test set', mean_squared_error(test_y, test_pred))
print('R2-Score of Testing set', r2_score(test_y, test_pred))
