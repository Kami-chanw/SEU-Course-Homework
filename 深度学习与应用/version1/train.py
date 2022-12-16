import scipy.io
import torch
import torch.nn as nn
import numpy as np
import matplotlib.pyplot as plt


def normalize(X):
    return (X - X.mean()) / X.std()


# Load experiment data
data = scipy.io.loadmat('../data_exp.mat')
F = torch.Tensor(data['F'])
time = torch.Tensor(data['time'])
x = torch.Tensor(data['x'])


# Define  network model
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


# Initialize model and optimizer
model = Net()
optimizer = torch.optim.Adam(model.parameters())

# Train the model
for epoch in range(30):
    for i in range(F.shape[0]):
        # Convert numpy arrays to tensors
        F_tensor = F[i]
        x_tensor = x[i]
        inputs = normalize(torch.cat((F_tensor, time.squeeze()), dim=0))

        # Forward pass
        output = normalize(model(inputs))
        # Compute loss and backpropagate
        loss = torch.nn.functional.mse_loss(output, x_tensor)
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()
    if (epoch + 1) % 10 == 0:
        print(f'Epoch {epoch + 1}: loss = {loss.item()}')
        
torch.save(model.state_dict(), "model.pt")