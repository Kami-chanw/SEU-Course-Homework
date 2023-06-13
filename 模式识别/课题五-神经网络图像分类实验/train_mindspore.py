from mindspore.dataset import MnistDataset
import utils
from utils_ms import *

train_dataset = MnistDataset('./data/train', shuffle=True)
test_dataset = MnistDataset('./data/test')

train_ds = ms_transform(train_dataset, 64)
test_ds = ms_transform(test_dataset, 64)

model = Network(28 * 28, 256, 10)

epochs = 10

history = model.train_and_eval(epochs, train_ds, test_ds)

utils.plot_history(history)
