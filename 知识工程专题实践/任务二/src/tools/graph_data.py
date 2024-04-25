from torch_geometric.data import Data, HeteroData
import torch
import pandas as pd
import numpy as np
from sentence_transformers import SentenceTransformer
from scipy.stats import kstest, shapiro


def _fill_data(patient_df, fill_mode, **kwargs):
    def sample_filling(data: pd.Series):
        return data.apply(
            lambda x: np.random.choice(data.dropna()) if pd.isna(x) else x
        )

    match fill_mode:
        case "default":
            _fill_data(patient_df, "constant", fill_value=-1, **kwargs)
        case "stats":
            missing_count = patient_df.isna().sum()
            for col in missing_count[missing_count != 0].index:
                if patient_df[col].nunique() < 10 or not pd.api.types.is_float_dtype(
                    patient_df[col]
                ):
                    patient_df[col] = sample_filling(patient_df[col])

                Q1 = patient_df[col].quantile(0.25)
                Q3 = patient_df[col].quantile(0.75)
                IQR = Q3 - Q1

                lower_bound = Q1 - 1.5 * IQR
                upper_bound = Q3 + 1.5 * IQR

                data_clean_iqr = patient_df[
                    (patient_df[col] >= lower_bound) & (patient_df[col] <= upper_bound)
                ]
                mean, std = (
                    data_clean_iqr[col].mean(),
                    data_clean_iqr[col].std(),
                )
                test_result = (
                    shapiro(data_clean_iqr[col].dropna())
                    if data_clean_iqr.shape[0] <= 50
                    else kstest(
                        data_clean_iqr[col].dropna(),
                        "norm",
                        args=(
                            mean,
                            std + 1e-8,  # prevent dividing zero
                        ),
                    )
                )

                if test_result.pvalue < 0.05:
                    patient_df[col] = sample_filling(patient_df[col])
                else:
                    patient_df.loc[patient_df[col].isna(), col] = np.random.normal(
                        mean,
                        std,
                        size=patient_df[col].isna().sum(),
                    )
        case "constant":
            if not "fill_value" in kwargs:
                raise ValueError(
                    "Constant value filling mode should specify `fill_value` in keyword arguments."
                )
            fill_value = kwargs["fill_value"]
            patient_df.fillna(fill_value, inplace=True)
        case "auto":
            raise NotImplementedError(
                """
                I have a genius idea about using an unsupervised method to fill in missing values, 
                which can take into account the diversity and the reality of the data.
                However, I don't have much time to implement.
                If you are interested in it, feel free to contact me!
                """
            )
        case _:
            raise ValueError("Unknown arguments for `fill_mode`")


def _get_id_mapping(patient_df, disease_df, medicine_df, is_homo):
    num_medicine_nodes = medicine_df.shape[0]
    num_patient_nodes = patient_df.shape[0]
    num_disease_nodes = disease_df.shape[0]

    disease_node_offset = num_patient_nodes if is_homo else 0
    medicine_node_offset = num_patient_nodes + num_disease_nodes if is_homo else 0

    return {
        "patient": dict(zip(patient_df.iloc[:, 0], range(num_patient_nodes))),
        "disease": dict(
            zip(
                disease_df.iloc[:, 0],
                range(disease_node_offset, disease_node_offset + num_disease_nodes),
            )
        ),
        "medicine": dict(
            zip(
                medicine_df.iloc[:, 0],
                range(
                    medicine_node_offset,
                    medicine_node_offset + num_medicine_nodes,
                ),
            )
        ),
    }


def _get_edge_index(dest_key, dest_df, mapping_df, id_mapping):
    # edges, see README.md to understand dataset format
    # column 0, 1 = id, name
    name_id_mapping = dict(zip(dest_df.iloc[:, 1], dest_df.iloc[:, 0]))
    # column 0, 1, 2 = id, relation, name
    src_id = mapping_df.iloc[:, -1].map(name_id_mapping).map(id_mapping[dest_key])

    return torch.tensor(
        pd.concat(
            [mapping_df.iloc[:, 0].map(id_mapping["patient"]), src_id],
            axis=1,
        )
        .dropna()
        .to_numpy(),
        dtype=torch.long,
    ).T


def homo_data(
    root,
    fill_mode="default",
    transform=None,
    return_id_mapping=False,
    **kwargs,
):

    patient_df = pd.read_csv(root + "patient-cleaned.csv", sep=",")

    _fill_data(patient_df, fill_mode, **kwargs)

    disease_df = pd.read_csv(root + "disease.csv", sep=",")
    medicine_df = pd.read_csv(root + "medicine.csv", sep=",")
    patient_disease_df = pd.read_csv(root + "patient-disease.csv", sep=",")
    patient_medicine_df = pd.read_csv(root + "patient-medicine.csv", sep=",")

    num_medicine_nodes = medicine_df.shape[0]
    num_patient_nodes = patient_df.shape[0]
    num_disease_nodes = disease_df.shape[0]
    num_features = patient_df.shape[1] - 1  # remove column ID

    # map <something>-id to node-id
    id_mapping = _get_id_mapping(patient_df, disease_df, medicine_df, is_homo=True)

    # node features
    x_tensor = torch.tensor(
        np.vstack(
            [
                patient_df.iloc[:, 1:].to_numpy(),  # remove column ID
                np.zeros((num_disease_nodes, num_features)),
                np.zeros((num_medicine_nodes, num_features)),
            ]
        ),
        dtype=torch.float,
    )

    # node labels
    node_labels = {node: label for label, node in enumerate(id_mapping.keys())}
    y_tensor = torch.cat(
        (
            torch.full(size=(num_patient_nodes,), fill_value=node_labels["patient"]),
            torch.full(size=(num_disease_nodes,), fill_value=node_labels["disease"]),
            torch.full(size=(num_medicine_nodes,), fill_value=node_labels["medicine"]),
        )
    )

    edge_index = torch.cat(
        [
            _get_edge_index("disease", disease_df, patient_disease_df, id_mapping),
            _get_edge_index("medicine", medicine_df, patient_medicine_df, id_mapping),
        ],
        dim=1,
    )

    data = Data(
        x=x_tensor,
        y=y_tensor,
        edge_index=edge_index,
        node_labels=node_labels,
    )

    if transform is not None:
        data = transform(data)
    if return_id_mapping == True:
        return data, id_mapping
    return data


def hetero_data(
    root,
    fill_mode="default",
    transform=None,
    return_id_mapping=False,
    device=None,
    **kwargs,
):

    patient_df = pd.read_csv(root + "patient-cleaned.csv", sep=",")

    _fill_data(patient_df, fill_mode, **kwargs)

    disease_df = pd.read_csv(root + "disease.csv", sep=",")
    medicine_df = pd.read_csv(root + "medicine.csv", sep=",")
    patient_disease_df = pd.read_csv(root + "patient-disease.csv", sep=",")
    patient_medicine_df = pd.read_csv(root + "patient-medicine.csv", sep=",")

    id_mapping = _get_id_mapping(patient_df, disease_df, medicine_df, is_homo=False)

    if device is None:
        device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    lm_model_name = kwargs.get("lm_model_name", "all-MiniLM-L6-v2")
    sentence_encoder = SentenceTransformer(lm_model_name, device=device)

    data = HeteroData()
    data["patient"].x = torch.tensor(
        patient_df.iloc[:, 1:].to_numpy(), dtype=torch.float
    )
    data["medicine"].x = torch.zeros((medicine_df.shape[0], medicine_df.shape[0]))
    data["disease"].x = sentence_encoder.encode(
        disease_df.iloc[:, 1], convert_to_tensor=True, device=device
    ).cpu()
    data["patient", "suffer", "disease"].edge_index = _get_edge_index(
        "disease", disease_df, patient_disease_df, id_mapping
    )
    data["patient", "take", "medicine"].edge_index = _get_edge_index(
        "medicine", medicine_df, patient_medicine_df, id_mapping
    )

    if transform is not None:
        data = transform(data)
    if return_id_mapping == True:
        return data, id_mapping
    return data
