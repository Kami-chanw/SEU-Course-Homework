from utils import *

batch_size = 64
epochs = 6

train_data = load_data('./data/train/train-images.idx3-ubyte',
                       './data/train/train-labels.idx1-ubyte', mnist_transform)
test_data = load_data('./data/test/t10k-images.idx3-ubyte',
                      './data/test/t10k-labels.idx1-ubyte', mnist_transform)

train_dl = MyDataLoader(train_data, batch_size, True)
test_dl = MyDataLoader(test_data, batch_size)

model = SingleLayerNN(28 * 28, 256, 10)
history = model.train_and_eval(epochs, train_dl, test_dl)
plot_history(history)
