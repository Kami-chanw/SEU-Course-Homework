import utils as u
import matplotlib.pyplot as plt
import mindspore
from mindspore.dataset import FashionMnistDataset
import utils_ms as ums

batch_size = 64
epochs = 10

train_data = u.load_data('./FashionMNIST/train/train-images-idx3-ubyte',
                       './FashionMNIST/train/train-labels-idx1-ubyte', u.mnist_transform)
test_data = u.load_data('./FashionMNIST/test/t10k-images-idx3-ubyte',
                      './FashionMNIST/test/t10k-labels-idx1-ubyte', u.mnist_transform)

train_dl = u.MyDataLoader(train_data, batch_size, True)
test_dl = u.MyDataLoader(test_data, batch_size)

model = u.SingleLayerNN(28 * 28, 256, 10)
history = model.train_and_eval(epochs, train_dl, test_dl)

train_ds = FashionMnistDataset('./FashionMNIST/train', shuffle=True)
test_ds = FashionMnistDataset('./FashionMNIST/test')
train_ds = ums.ms_transform(train_ds, 64, mean=0.2860, std=0.3530)
test_ds = ums.ms_transform(test_ds, 64, mean=0.2860, std=0.3530)

model_ms = ums.Network(28 * 28, 256, 10)
history_ms = model_ms.train_and_eval(epochs, train_ds, test_ds)
u.plot_history(MyNet=history, MindSpore=history_ms)
