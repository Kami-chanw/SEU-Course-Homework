import torch
from torch import nn
from torchvision import datasets
from config import args as arg
from utils import *

batch_size = arg.batch_size
lr = arg.lr
max_lr = 10 * lr
grad_clip = 0.1
epochs = arg.epochs
momentum = arg.momentum
weight_decay = arg.weight_decay
seed = arg.seed

torch.manual_seed(seed)

train_data = datasets.FashionMNIST(root="data",
                                   train=True,
                                   download=True,
                                   transform=train_tfms)

test_data = datasets.FashionMNIST(root="data",
                                  train=False,
                                  download=True,
                                  transform=test_tfms)

train_dl = CudaDataLoader(train_data,
                          batch_size=batch_size,
                          shuffle=True,
                          pin_memory=True,
                          num_workers=2)
test_dl = CudaDataLoader(test_data,
                                 batch_size=batch_size,
                                 pin_memory=True,
                                 num_workers=2)


class SingleLayerNN(nn.Module):
    def __init__(self, num_inputs, num_outputs, num_hiddens, initializer = None):
        super().__init__()
        self.num_inputs, self.num_outputs, self.num_hiddens = num_inputs, num_outputs, num_hiddens
        self.flatten = nn.Flatten()
        self.fc1 = nn.Linear(num_inputs, num_hiddens)
        self.relu = nn.ReLU()
        self.fc2 = nn.Linear(num_hiddens, num_outputs)
        if initializer:
            initializer(self.fc1.weight)
            initializer(self.fc2.weight)
        

    def forward(self, x):
        x = self.flatten(x)
        x = self.fc1(x)
        x = self.relu(x)
        x = self.fc2(x)
        return x


class TrainStep:
    def __init__(self,
                 model,
                 lr=lr,
                 max_lr=max_lr,
                 epochs=epochs,
                 momentum=momentum,
                 weight_decay=weight_decay):
        self.loss_fn = nn.CrossEntropyLoss()
        self.optimizer = torch.optim.SGD(model.parameters(),
                                         lr=lr,
                                         momentum=momentum,
                                         weight_decay=weight_decay)
        self.sched = torch.optim.lr_scheduler.OneCycleLR(
            self.optimizer,
            max_lr,
            epochs=epochs,
            steps_per_epoch=len(train_dl))
    
    def __call__(self, model, images, labels):
        self.optimizer.zero_grad()
        loss = self.loss_fn(model(images), labels)
        loss.backward()
        nn.utils.clip_grad_value_(model.parameters(), grad_clip)
        self.optimizer.step()
        self.sched.step()
        return loss


if __name__ == "__main__":
    model = to_device(SingleLayerNN(784, 10, 256), get_default_device())

    history = fit_and_test(model, epochs, train_dl, test_dl, TrainStep(model))
    torch.save(model.state_dict(), 'single_layer_nn.pth')
