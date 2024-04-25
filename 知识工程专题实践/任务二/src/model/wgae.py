import torch
import torch.nn.functional as F
from torch_geometric.nn import GCNConv


# Use Wasserstein graph auto encoder to contrust robust latent representation, which might be a good approach.
# I don't have spare time to implement it, if you are interested in it, feel free to contact me.
class WGAE(torch.nn.Module):
    pass
