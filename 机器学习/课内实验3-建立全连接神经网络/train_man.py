import torch
from torch import nn
from utils import *
from torchvision import datasets
from config import args as arg
import torch.nn.functional as F

batch_size = arg.batch_size
lr = arg.lr
grad_clip = 0.1
epochs = arg.epochs
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


class SingleLayerNN:
    def __init__(self,
                 input_size,
                 hidden_size,
                 output_size,
                 learning_rate=0.1):
        self.input_size = input_size
        self.hidden_size = hidden_size
        self.output_size = output_size
        self.learning_rate = learning_rate
        self.device = torch.device('cpu')

        self.w1 = torch.randn(input_size, hidden_size) * 0.01
        self.b1 = torch.zeros(hidden_size)
        self.w2 = torch.randn(hidden_size, output_size) * 0.01
        self.b2 = torch.zeros(output_size)
    
    def __call__(self, inputs):
        return self.forward(inputs)

    def forward(self, x):
        x = x.view(-1, self.w1.shape[0])
        h1 = relu(x @ self.w1 + self.b1)
        return h1 @ self.w2 + self.b2

    def parameters(self):
        yield self.w1
        yield self.b1
        yield self.w2
        yield self.b2

    def named_parameters(self):
        params = {'w1': self.w1, 'b1': self.b1, 'w2': self.w2, 'b2': self.b2}
        return params.items()

    def to(self, device, non_blocking=False):
        self.device = device
        for name, param in self.named_parameters():
            setattr(self, name, param.to(device, non_blocking=non_blocking))
        return self


class TrainStep:
    def __call__(self, model, images, labels):
        # forwardw
        outputs = model(images)
        z = images.view(-1, model.w1.shape[0]) @ model.w1 + model.b1

        # calculate loss
        loss = F.cross_entropy(outputs, labels)

        # backpropagation
        delta_2 = F.softmax(outputs, dim=1) - F.one_hot(labels,
                                                        num_classes=10)
        grad_w2 = relu(z).T @ delta_2 / batch_size
        grad_b2 = torch.sum(delta_2, dim=0) / batch_size

        delta_1 = delta_2 @ model.w2.T
        delta_1[z <= 0] = 0
        grad_w1 = images.view(-1, model.w1.shape[0]).T @ delta_1 / batch_size
        grad_b1 = torch.sum(delta_1, dim=0) / batch_size

        # parameters update
        model.w2 -= model.learning_rate * grad_w2
        model.b2 -= model.learning_rate * grad_b2
        model.w1 -= model.learning_rate * grad_w1
        model.b1 -= model.learning_rate * grad_b1
        nn.utils.clip_grad_value_(model.parameters(), grad_clip)

        return loss


if __name__ == "__main__":
    history = []
    model = to_device(
        SingleLayerNN(input_size=784,
                      hidden_size=256,
                      output_size=10,
                      learning_rate=lr), get_default_device())
    history = fit_and_test(model, epochs, train_dl, test_dl, TrainStep())
