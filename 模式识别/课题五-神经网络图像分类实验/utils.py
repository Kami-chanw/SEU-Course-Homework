import numpy as np
import matplotlib.pyplot as plt


def mnist_transform(images):
    return ((images - np.mean(images)) / np.std(images)) / 255

def plot_history(*args, **kwargs):
    fig = plt.figure(figsize=(8, 4))
    ax1 = fig.add_subplot(1, 2, 1)
    ax2 = fig.add_subplot(1, 2, 2)
    if len(args) > 0 and len(kwargs) > 0:
        raise RuntimeError(
            "cannot specify position args and keyword args at the same time")

    data = zip([""] * len(args), args) if len(args) > 0 else kwargs.items()
    for label, his in data:
        ax1.plot([x['train_loss'] for x in his], label=label + ' train loss')
        ax1.plot([x['test_loss'] for x in his], label=label + ' test loss')
        ax2.plot([x['test_acc'] for x in his], label=label + ' accuracy')
    ax2.set_ylabel("Accuracy")
    ax2.set_title('Accuracy on test set vs Epochs')
    ax1.set_ylabel('Loss')
    ax1.set_title('Loss vs Epochs')
    ax1.legend()
    ax2.legend()
    fig.supxlabel("Epochs")
    fig.tight_layout()
    plt.show()


def load_data(image_path,
              label_path,
              image_transform=None,
              label_transform=None):
    with open(image_path, "rb") as f:
        image = np.frombuffer(f.read(), np.uint8,
                              offset=16).reshape(-1, 28, 28)
    with open(label_path, "rb") as f:
        label = np.frombuffer(f.read(), np.uint8, offset=8)
    if image_transform is not None:
        image = image_transform(image)
    if label_transform is not None:
        label = label_transform(label)
    return image, label


class MyDataLoader:
    def __init__(self, data, batch_size=1, shuffle=False):
        self.batch_size = batch_size
        self.shuffle = shuffle

        self.image, self.label = data
        self.num_samples = len(self.image)
        self.indices = np.arange(self.num_samples)
        if self.shuffle:
            np.random.shuffle(self.indices)

    def __iter__(self):
        self.current_idx = 0
        return self

    def __next__(self):
        if self.current_idx >= self.num_samples:
            raise StopIteration
        indices = self.indices[self.current_idx:self.current_idx +
                               self.batch_size]
        images = self.image[indices]
        labels = self.label[indices]
        self.current_idx += self.batch_size
        return images, labels


def one_hot(y, num_classes):
    return np.eye(num_classes)[y].astype(float)


def relu(x):
    return np.maximum(0, x)


def softmax(x, axis):
    x = x - np.max(x, axis=axis, keepdims=True)
    return np.exp(x) / np.sum(np.exp(x), axis=axis, keepdims=True)


def cross_entropy(y_pred, y_true):
    epsilon = 1e-10
    y_pred = np.clip(y_pred, epsilon, 1.0 - epsilon)
    return -np.mean(one_hot(y_true, y_pred.shape[1]) * np.log(y_pred))


class SingleLayerNN:
    def __init__(self, input_size, hidden_size, output_size, lr=1e-2):
        self.input_size = input_size
        self.hidden_size = hidden_size
        self.output_size = output_size
        self.lr = lr

        self.w1 = np.random.randn(input_size, hidden_size) * 0.01
        self.b1 = np.zeros(hidden_size)
        self.w2 = np.random.randn(hidden_size, output_size) * 0.01
        self.b2 = np.zeros(output_size)

    def __call__(self, inputs):
        return self.forward(inputs)

    def forward(self, x):
        x = x.reshape(-1, self.w1.shape[0])
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

    def _train_step(self, images, labels, optimizer=None):
        batch_size = images.shape[0]
        # forward
        outputs = self(images)
        z = images.reshape(-1, self.w1.shape[0]) @ self.w1 + self.b1

        # calculate loss
        loss = cross_entropy(outputs, labels)

        # backpropagation
        delta_2 = softmax(outputs, axis=1) - one_hot(labels, num_classes=10)
        grad_w2 = relu(z).T @ delta_2 / batch_size
        grad_b2 = np.sum(delta_2, axis=0) / batch_size

        delta_1 = delta_2 @ self.w2.T
        delta_1[z <= 0] = 0
        grad_w1 = images.reshape(-1, self.w1.shape[0]).T @ delta_1 / batch_size
        grad_b1 = np.sum(delta_1, axis=0) / batch_size

        # parameters update
        if optimizer is None:
            self.w2 -= self.learning_rate * grad_w2
            self.b2 -= self.learning_rate * grad_b2
            self.w1 -= self.learning_rate * grad_w1
            self.b1 -= self.learning_rate * grad_b1
        else:
            optimizer.step([grad_w1, grad_b1, grad_w2, grad_b2])

        return loss

    def train_and_eval(self, epochs, train_dl, test_dl):
        history = []
        optim = Adam(self.parameters(), lr=self.lr)
        for epoch in range(epochs):
            train_losses = []
            for images, labels in train_dl:
                train_losses.append(self._train_step(images, labels, optim))

            test_losses = []
            test_accs = []
            for images, labels in test_dl:
                outputs = self(images)
                test_losses.append(cross_entropy(outputs, labels))
                preds = np.argmax(outputs, axis=1)
                test_accs.append(
                    np.array(np.sum(preds == labels).item() / len(preds)))

            history.append({
                'train_loss': np.stack(train_losses).mean().item(),
                'test_loss': np.stack(test_losses).mean().item(),
                'test_acc': np.stack(test_accs).mean()
            })
            print(
                f"Epoch[{epoch + 1:d}]: train_loss: {history[-1]['train_loss']:.4f}, test_loss: {history[-1]['test_loss']:.4f}, test_acc: {history[-1]['test_acc']:.4f}"
            )
        return history


class Adam():
    def __init__(self, params, lr=1e-2, betas=(0.9, 0.999), eps=1e-8):
        self.params = list(params)
        self.lr = lr
        self.betas = betas
        self.eps = eps
        self.t = 0
        self.m = [np.zeros_like(p) for p in self.params]
        self.v = [np.zeros_like(p) for p in self.params]

    def step(self, grads):
        self.t += 1.
        for i, p, grad in zip(range(len(grads)), self.params, grads):
            self.m[i] = self.betas[0] * self.m[i] + (1 - self.betas[0]) * grad
            self.v[i] = self.betas[1] * self.v[i] + (1 -
                                                     self.betas[1]) * grad**2
            m_hat = self.m[i] / (1 - self.betas[0]**self.t)
            v_hat = self.v[i] / (1 - self.betas[1]**self.t)
            p -= self.lr * m_hat / (np.sqrt(v_hat) + self.eps)
