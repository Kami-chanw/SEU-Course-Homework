import torch
from torch_geometric.data import Data, HeteroData
from typing import List, Union, Tuple
import copy


def split_homo_graph(
    data: Data, nodes: Union[List[int], torch.Tensor]
) -> Tuple[Data, Data]:
    device = data.x.device
    nodes = torch.as_tensor(nodes, dtype=torch.long, device=device)

    mask_subgraph_edges = torch.isin(data.edge_index, nodes).any(dim=0)
    edge_index_subgraph = data.edge_index[:, mask_subgraph_edges]

    subgraph_nodes = torch.unique(edge_index_subgraph)
    x_subgraph = data.x[subgraph_nodes]
    y_subgraph = data.y[subgraph_nodes] if data.y is not None else None

    subgraph_node_mapping = {
        old_idx.item(): new_idx for new_idx, old_idx in enumerate(subgraph_nodes)
    }
    subgraph_edges = torch.tensor(
        [
            [subgraph_node_mapping[idx.item()] for idx in edge]
            for edge in edge_index_subgraph.t()
        ],
        device=device,
    )

    edge_index_rest = data.edge_index[:, ~mask_subgraph_edges]

    all_nodes = torch.arange(data.num_nodes, device=data.edge_index.device)
    rest_nodes = all_nodes[~torch.isin(all_nodes, nodes)]
    x_rest = data.x[rest_nodes]
    y_rest = data.y[rest_nodes] if data.y is not None else None

    rest_node_mapping = {
        old_idx.item(): new_idx for new_idx, old_idx in enumerate(rest_nodes)
    }
    rest_edges = torch.tensor(
        [
            [rest_node_mapping[idx.item()] for idx in edge]
            for edge in edge_index_rest.t()
        ],
        device=device,
    )

    subgraph_data, rest_data = copy.deepcopy(data), copy.deepcopy(data)
    return subgraph_data.update(
        {"x": x_subgraph, "y": y_subgraph, "edge_index": subgraph_edges.t()}
    ), rest_data.update({"x": x_rest, "y": y_rest, "edge_index": rest_edges.t()})


def split_hetero_graph(
    data: HeteroData, node_type: str, nodes: Union[List[int], torch.Tensor]
) -> Tuple[HeteroData, HeteroData]:
    device = data.x_dict[node_type].device
    nodes = torch.as_tensor(nodes, dtype=torch.long, device=device)

    subgraph_data, rest_data = copy.deepcopy(data), copy.deepcopy(data)

    node_mappings = {}
    for n_type in data.node_types:
        all_nodes = torch.arange(data[n_type].num_nodes, device=device)
        if n_type == node_type:
            subgraph_nodes = nodes
            rest_nodes = all_nodes[~torch.isin(all_nodes, nodes)]
        else:
            subgraph_nodes = all_nodes
            rest_nodes = all_nodes

        node_mappings[n_type] = {
            "subgraph": {
                old_idx.item(): new_idx
                for new_idx, old_idx in enumerate(subgraph_nodes)
            },
            "rest": {
                old_idx.item(): new_idx for new_idx, old_idx in enumerate(rest_nodes)
            },
        }

        subgraph_data[n_type].x = data[n_type].x[subgraph_nodes]
        rest_data[n_type].x = data[n_type].x[rest_nodes]

    for rel in data.edge_types:
        src_type, _, dst_type = rel
        edge_index = data[rel].edge_index

        if src_type == node_type:
            mask = torch.isin(edge_index[0], nodes)
        if dst_type == node_type:
            mask = (
                torch.isin(edge_index[1], nodes)
                if src_type != node_type
                else mask | torch.isin(edge_index[1], nodes)
            )

        edge_index_sub = edge_index[:, mask]
        subgraph_data[rel].edge_index = torch.stack(
            [
                torch.tensor(
                    [
                        node_mappings[src_type]["subgraph"][idx.item()]
                        for idx in edge_index_sub[0]
                    ]
                ),
                torch.tensor(
                    [
                        node_mappings[dst_type]["subgraph"][idx.item()]
                        for idx in edge_index_sub[1]
                    ]
                ),
            ],
            dim=0,
        ).to(device=device)

        edge_index_rest = edge_index[:, ~mask]
        rest_data[rel].edge_index = torch.stack(
            [
                torch.tensor(
                    [
                        node_mappings[src_type]["rest"][idx.item()]
                        for idx in edge_index_rest[0]
                    ]
                ),
                torch.tensor(
                    [
                        node_mappings[dst_type]["rest"][idx.item()]
                        for idx in edge_index_rest[1]
                    ]
                ),
            ],
            dim=0,
        ).to(device=device)

    return subgraph_data, rest_data
