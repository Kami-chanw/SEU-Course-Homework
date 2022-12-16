import torch
import torch.nn as nn
from scipy.io import loadmat
import matplotlib.pyplot as plt

def normalize(X):
    return (X - X.mean()) / X.std()
# 加载数据
data = loadmat("../data_pred2.mat")
F = torch.Tensor(data['F'])
time = torch.Tensor(data['time']).squeeze()
x = torch.Tensor(data['x'])


# 定义神经网络
class Net(nn.Module):

    def __init__(self):
        super(Net, self).__init__()
        self.fc1 = nn.Linear(2000, 1000)
        self.fc2 = nn.Linear(1000, 1000)
        self.fc3 = nn.Linear(1000, 1000)

    def forward(self, x):
        x = torch.relu(self.fc1(x))
        x = torch.relu(self.fc2(x))
        x = self.fc3(x)
        return x


# 加载训练好的模型
net = Net()
net.load_state_dict(torch.load("model.pt"))

# 预测结构位移
for i in range(F.shape[0]):
    inputs = torch.cat((normalize(F[i]), normalize(time)), dim=0)
    output = normalize(net(inputs))
    plt.plot(time.detach(), output.detach(), label=f"pred-{i}")
    plt.plot(time.detach(), normalize(x[i].detach()), label=f"real-{i}")
    plt.legend()
    plt.xlabel("time")
    plt.ylabel("x")
    plt.show()

