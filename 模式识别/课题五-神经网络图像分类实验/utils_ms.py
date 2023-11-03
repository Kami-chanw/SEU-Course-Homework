import mindspore
from mindspore import nn
from mindspore import ops
from mindspore.dataset import vision, transforms
from mindspore.dataset import MnistDataset

def ms_transform(dataset, batch_size, mean=0.1307, std=0.3081):
    image_transforms = [
        vision.Rescale(1.0 / 255.0, 0),
        vision.Normalize(mean=(mean,), std=(std,)),
        vision.HWC2CHW()
    ]
    label_transform = transforms.TypeCast(mindspore.int32)

    dataset = dataset.map(image_transforms, 'image')
    dataset = dataset.map(label_transform, 'label')
    dataset = dataset.batch(batch_size)
    return dataset

class Network(nn.Cell):
    def __init__(self,input_size, hidden_size, output_size, lr=1e-2):
        super().__init__()
        self.flatten = nn.Flatten()
        self.fc1 = nn.Dense(input_size, hidden_size)
        self.relu = nn.ReLU()
        self.fc2 = nn.Dense(hidden_size, output_size)
        self.loss_fn = nn.CrossEntropyLoss()
        self.optimizer = nn.SGD(self.trainable_params(), lr)

    def construct(self, x):
        x = self.flatten(x)
        x = self.fc1(x)
        x = self.relu(x)
        x = self.fc2(x)
        return x

    def forward(self, data, label):
        logits = self(data)
        loss = self.loss_fn(logits, label)
        return loss, logits

    def train_and_eval(self, epochs, train_ds, test_ds):
        history = []
        for i in range(epochs):
            grad_fn = ops.value_and_grad(self.forward,
                                         None,
                                         self.optimizer.parameters,
                                         has_aux=True)

            self.set_train()
            num_batches = train_ds.get_dataset_size()
            train_loss = 0
            for data, label in train_ds.create_tuple_iterator():
                (loss, _), grads = grad_fn(data, label)
                train_loss += ops.depend(loss, self.optimizer(grads))

            self.set_train(False)
            num_batches = test_ds.get_dataset_size()
            total, test_loss, correct = 0, 0, 0
            for data, label in test_ds.create_tuple_iterator():
                pred = self(data)
                total += len(data)
                test_loss += self.loss_fn(pred, label).asnumpy()
                correct += (pred.argmax(1) == label).asnumpy().sum()
            history.append({
                'train_loss': float(train_loss / num_batches),
                'test_loss': float(test_loss / num_batches),
                'test_acc': float(correct / total)
            })
            print(
                f"Epoch[{i + 1:d}]: train_loss: {history[-1]['train_loss']:.4f}, test_loss: {history[-1]['test_loss']:.4f}, test_acc: {history[-1]['test_acc']:.4f}"
            )
        return history