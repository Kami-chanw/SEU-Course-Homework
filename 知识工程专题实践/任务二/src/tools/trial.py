from typing import List, Union
import torch
from sklearn.metrics import roc_auc_score
from torch_geometric.metrics import LinkPredF1, LinkPredPrecision, LinkPredRecall
from torch_geometric.data import Data, HeteroData
from torch_geometric import transforms as T
import os


class Study:
    def _unpack_data(self, data):
        if isinstance(data, Data):
            x = data.x
            edge_index = data.edge_index
            edge_label_index = data.edge_label_index
            gt = data.edge_label
        else:
            x = data.x_dict
            edge_index = data.edge_index_dict
            edge_label_index = data["patient", "medicine"].edge_label_index
            gt = data["patient", "medicine"].edge_label
        return x, edge_index, gt, edge_label_index

    @torch.no_grad()
    def _eval(self, data):
        self.model.eval()

        x, edge_index, gt, edge_label_index = self._unpack_data(data)

        z = self.model.encode(x, edge_index)
        out = self.model.decode(z, edge_label_index).sigmoid()
        return roc_auc_score(gt.cpu().numpy(), out.cpu().numpy())

    def _train(self, optimizer, criterion, scheduler=None):
        self.model.train()
        optimizer.zero_grad()

        x, edge_index, gt, edge_label_index = self._unpack_data(self.train_data)

        z = self.model.encode(x, edge_index)
        out = self.model.decode(z, edge_label_index).view(-1)
        loss = criterion(out.sigmoid(), gt)
        loss.backward()
        optimizer.step()
        if scheduler is not None:
            scheduler.step()
        return loss

    def __init__(
        self, model, train_graph: Union[Data, HeteroData], split: T.RandomLinkSplit
    ) -> None:
        assert (
            split.neg_sampling_ratio == 0 or split.add_negative_train_samples
        ), "Must do negative sampling in split"

        self.model = model
        self.train_data, self.val_data, self.test_data = split(train_graph)

    def train(
        self, epoch, optimizer, criterion, scheduler=None, verbose=0, save_dir=None
    ):
        self.loss = []
        self.valid_auc = []
        self.test_auc = []
        if not os.path.exists(save_dir):
            os.makedirs(save_dir)
        best_valid_auc = final_test_auc = 0
        for e in range(epoch):
            loss = self._train(optimizer, criterion, scheduler)
            valid_auc = self._eval(self.val_data)
            test_auc = self._eval(self.test_data)
            self.loss.append(loss)
            self.valid_auc.append(valid_auc)
            self.test_auc.append(test_auc)

            if valid_auc > best_valid_auc:
                best_valid_auc = valid_auc
                final_test_auc = test_auc

            if verbose == 0:
                print(
                    f"Epoch: {e + 1:d}/{epoch:d}, Loss: {loss:.4f}, Val: {valid_auc:.4f}, "
                    f"Test: {test_auc:.4f}"
                )
            if save_dir is not None:
                torch.save(
                    self.model.state_dict(), f"{save_dir}/{self.model.name}_{e + 1}.pt"
                )

        if verbose <= 1:
            print(f"Final test auc: {final_test_auc:.4f}")

    @torch.no_grad
    def test(self, data, topk):
        assert type(self.train_data) == type(data), "Data type mismatch"
        self.model.eval()
        if isinstance(data, Data):
            medicine_indices = (data.y == data.node_labels["medicine"]).nonzero(
                as_tuple=True
            )[0]
            patient_indices = (data.y == data.node_labels["patient"]).nonzero(
                as_tuple=True
            )[0]
            gt = data.edge_index[
                :,
                torch.isin(data.edge_index[1], medicine_indices)
                & torch.isin(data.edge_index[0], patient_indices),
            ]

            z = self.model.encode(data.x, data.edge_index)
            out = z @ z.t()

            pred_index_mat = medicine_indices[
                torch.topk(
                    out[patient_indices][:, medicine_indices], topk, dim=1
                ).indices
            ]
        else:
            gt = data["patient", "take", "medicine"].edge_index
            patient_indices = torch.arange(data["patient"].num_nodes).to(gt.device)
            medicine_indices = torch.arange(data["medicine"].num_nodes).to(gt.device)

            z_dict = self.model.encode(data.x_dict, data.edge_index_dict)

            out = z_dict["patient"] @ z_dict["medicine"].T

            pred_index_mat = medicine_indices[torch.topk(out, topk, dim=1).indices]

        device = gt.device
        precision, recall, f1 = (
            LinkPredPrecision(topk).to(device),
            LinkPredRecall(topk).to(device),
            LinkPredF1(topk).to(device),
        )

        precision.update(pred_index_mat, gt)
        recall.update(pred_index_mat, gt)
        f1.update(pred_index_mat, gt)
        return precision.compute(), recall.compute(), f1.compute()

    def find_best(self, ckpt_dir, test_graph, n_trials, model_ids, topks):
        import optuna

        def objective(trial):
            epoch = trial.suggest_categorical("model_id", model_ids)
            topk = trial.suggest_categorical("topk", topks)
            ckpt = torch.load(f"{ckpt_dir}{self.model.name}_{epoch}.pt")
            self.model.load_state_dict(ckpt)
            _, _, f1_mean = self.test(test_graph, topk)

            return f1_mean

        study = optuna.create_study(direction="maximize")

        study.optimize(objective, n_trials=n_trials)

        print("Best F1 score:", study.best_value)
        print("Optimal parameters:", study.best_params)


class AblationStudy:
    studies = {}

    def add(self, name: str, study: Study):
        self.studies[name] = study

    def remove(self, name: str):
        del self.studies[name]

    def plot(self, study_names: List[str], items: Union[List[str], str]):
        if isinstance(items, str):
            items = [items]
