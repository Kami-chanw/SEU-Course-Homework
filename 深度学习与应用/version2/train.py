from sklearn.linear_model import ElasticNet
import numpy as np
from scipy.io import loadmat
import matplotlib.pyplot as plt


def normalize(X):
    return (X - X.mean()) / X.std()


# 加载数据
data = loadmat("../data_exp.mat")
F = data['F']
time = data['time']
x = data['x']

# 训练数据
X = F + time
y = x

# 创建ElasticNet模型
model = ElasticNet()

# 训练模型
model.fit(X, y)

data = loadmat("../data_pred2.mat")
F = data['F']
time = data['time']
x = data['x']

# 使用模型进行预测
predictions = model.predict(X)
plt.plot(time, predictions, label=f"pred")
plt.plot(time, normalize(x[-1]), label=f"real")
plt.legend()
plt.xlabel("time")
plt.ylabel("x")
plt.show()