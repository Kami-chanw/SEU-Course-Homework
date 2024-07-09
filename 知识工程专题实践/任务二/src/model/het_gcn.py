import torch
import torch.nn as nn
import torch.nn.functional as F
from torch_geometric.nn import SAGEConv, Linear, to_hetero


class SAGEEncoder(torch.nn.Module):
    def __init__(self, hidden_channels, out_channels):
        super().__init__()
        self.conv1 = SAGEConv((-1, -1), hidden_channels)
        self.conv2 = SAGEConv((-1, -1), out_channels)
        self.dropout = nn.Dropout()

    def forward(self, x, edge_index):
        x = self.conv1(x, edge_index).relu()
        x = self.dropout(x)
        return self.conv2(x, edge_index)


class EdgeDecoder(torch.nn.Module):
    def __init__(self, hidden_channels):
        super().__init__()
        self.lin1 = Linear(2 * hidden_channels, hidden_channels)
        self.lin2 = Linear(hidden_channels, 1)

    def forward(self, z_dict, edge_label_index):
        row, col = edge_label_index
        z = torch.cat([z_dict["patient"][row], z_dict["medicine"][col]], dim=-1)
        z = self.lin1(z).relu()
        return self.lin2(z)


class HetGCN(torch.nn.Module):
    def __init__(self, metadata, hidden_channels):
        super().__init__()
        self.name = "HetGCN"
        self.encoder = SAGEEncoder(hidden_channels, hidden_channels)
        self.encoder = to_hetero(self.encoder, metadata, aggr="sum")
        self.decoder = EdgeDecoder(hidden_channels)

    def encode(self, x_dict, edge_index_dict):
        return self.encoder(x_dict, edge_index_dict)

    def decode(self, z_dict, edge_label_index):
        return self.decoder(z_dict, edge_label_index)
