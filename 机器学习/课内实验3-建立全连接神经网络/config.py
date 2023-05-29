import argparse

parser = argparse.ArgumentParser(description='Hyper-parameters management')

# Hardware options
parser.add_argument('--n_threads',
                    type=int,
                    default=6,
                    help='number of threads for data loading')
parser.add_argument('--cpu', action='store_true', help='use cpu only')
parser.add_argument('--seed', type=int, default=42, help='random seed')

# data in/out and dataset

parser.add_argument('--save',
                    default='./',
                    help='save path of trained model')

parser.add_argument('--predict',
                    default='./',
                    help='save path of predict model')

parser.add_argument('--batch_size',
                    type=int,
                    default=64,
                    help='batch size of trainset')

# train
parser.add_argument('--epochs',
                    type=int,
                    default=10,
                    metavar='N',
                    help='number of epochs to train (default: 10)')

parser.add_argument('--lr',
                    type=float,
                    default=0.01,
                    metavar='LR',
                    help='learning rate (default: 0.01)')

parser.add_argument('--momentum',
                    type=float,
                    default=0.5,
                    metavar='M',
                    help='SGD momentum (default: 0.5)')

parser.add_argument('--weight_decay',
                    type=float,
                    default=1e-4,
                    metavar='W',
                    help='SGD weight_decay (default: 1e-4)')

parser.add_argument('--nesterov',
                    type=bool,
                    default=False,
                    help='SGD nesterov (default: False)')

parser.add_argument('--early-stop',
                    default=20,
                    type=int,
                    help='early stopping (default: 20)')

args, unknown = parser.parse_known_args()
