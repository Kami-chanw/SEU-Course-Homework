import torch
import torch.nn as nn
import torch.nn.functional as F
from torch_geometric.nn import HANConv, Linear, to_hetero
from .het_gcn import EdgeDecoder


class HAN(torch.nn.Module):
    def __init__(self, metadata, hidden_channels, head=8):
        super().__init__()
        self.name = "HAN"
        self.han = HANConv(-1, hidden_channels, metadata, head, dropout=0.2)
        self.decoder = EdgeDecoder(hidden_channels)

    def encode(self, x_dict, edge_index_dict):
        return self.han(x_dict, edge_index_dict)

    def decode(self, z_dict, edge_label_index):
        return self.decoder(z_dict, edge_label_index)
