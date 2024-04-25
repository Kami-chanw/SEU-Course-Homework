import torch
import torch.nn.functional as F
from torch.nn import Linear, BatchNorm1d, ReLU
from torch_geometric.nn import GCNConv, DeepGCNLayer


class DeepGCN(torch.nn.Module):
    def __init__(self, in_channels, hidden_channels, out_channels, num_layers):
        super().__init__()
        self.name = "DeepGCN"
        self.node_encoder = Linear(in_channels, hidden_channels)

        self.layers = torch.nn.ModuleList()
        for i in range(num_layers):
            conv = GCNConv(hidden_channels, hidden_channels)
            norm = BatchNorm1d(hidden_channels)
            act = ReLU(inplace=True)

            layer = DeepGCNLayer(conv, norm, act, block="res+", dropout=0.2)
            self.layers.append(layer)

        self.lin = Linear(hidden_channels, out_channels)

    def encode(self, x, edge_index):
        x = self.node_encoder(x)

        for layer in self.layers:
            x = layer(x, edge_index)

        x = self.layers[-1].act(self.layers[-1].norm(x))
        x = F.dropout(x, p=0.1, training=self.training)

        return self.lin(x)

    def decode(self, z, edge_label_index):
        return (z[edge_label_index[0]] * z[edge_label_index[1]]).sum(dim=-1)
