# 糖尿病带药预测任务

本仓库包含东南大学人工智能专业21级知识工程专题实践任务二——糖尿病带药预测任务的实现代码。如对代码实现有疑问、发现问题或**彩蛋**，欢迎open issues。

## 代码组织
### notebooks
[`data_preprocessing.ipynb`](./notebooks/data_preprocessing.ipynb)：用于数据预处理，并形成同构图和异构图的PyTorch Geometric所需的表示。

[`train.ipynb`](./notebooks/train.ipynb)：用于在同构图上训练。

[`train_hetero.ipynb`](./notebooks/train_hetero.ipynb)：用于在异构图上训练。

### data
此目录包含本实验所需的数据集（经由提取和清洗，并从知识图谱中导出），数据集的格式描述见[此](#数据集描述)。
| Files                                                 | Description                                                                        |
| ----------------------------------------------------- | ---------------------------------------------------------------------------------- |
| [`disease.csv`](./data/disease.csv)                   | 所有疾病ID及名称                                                                   |
| [`medicine.csv`](./data/medicine.csv)                 | 所有药品ID及名称                                                                   |
| [`patinet-cleaned`](./data/patient-cleaned.csv)       | 预处理后的病人特征，由[`data_preprocessing.ipynb`](./data_preprocessing.ipynb)生成 |
| [`patient-disease.csv`](./data/patient-disease.csv)   | 病人-病名的边                                                                      |
| [`patient-medicine.csv`](./data/patient-medicine.csv) | 病人-药名的边                                                                      |
| [`patient.csv`](./data/patient.csv)                   | 未处理的病人特征                                                                   |

### src
此目录包含模型和通用工具的实现代码。
#### model
实现了两个同构图网络：GCN<sup><a href="#kipf2016semi">4</a></sup>、GAT<sup><a href="#velivckovic2017graph">5</a></sup>和DeepGCN<sup><a href="#li2019deepgcn">3</a></sup>，三个异构图网络：HetGCN、HAN<sup><a href="#wang2019heterogeneous">6</a></sup>。
#### tools
实现了同构图和异构图的数据表示（[`graph_data.py`](./src/tools/graph_data.py)），实用工具（[`utils.py`](/src/tools/utils.py)）和实验所需的工具（[`trial.py`](./src/tools/trial.py)）
## 数据集格式
`disease.csv`、`medicine`第一列为ID，第二列为名称。

`patient.csv`第一列为ID，其余列为特征。其中缺失值必须留空或以`-1`填充。

`patient-disease.csv`、`patient-medicine.csv`第一列为病人ID，第二列为名称。

*注：上述所有的ID不一定要从0开始，也不一定要连续。*
## 实现细节 Q & A
### Q: 为什么要用GNN？
这显然是一个链接预测问题——输入是一个实体为病人、药物和疾病，关系为患病和带药的图，输出预测病人和各个药物之间是否存在边。而GNN恰好擅长于处理结构化的数据，并且已经被成功地用于了知识图谱中<sup><a href="#ijcai2017p250">1</a></sup>。此外，本数据集地规模（9k+ 个结点和 50k+ 条边）也允许神经网络从数据中学到潜在分布。
### Q: 为什么GCN都这么浅？
在视觉任务中，我们有一种直觉：模型的复杂度越高，我们的模型就会有更高的拟合效果。然而在GCN当中，通常浅层（例如2 ~ 3层）GCN就有不错的效果，而深层（例如4 ~ 5层）反而效果变差，节点的区分性越来越差，其表示向量趋于一致，出现梯度消失、过平滑、过拟合等一系列问题，这使得相关学习任务难以继续进行。

那为什么会出现这样的现象呢？GCN 主要聚合邻节点的信息，对于任意一个节点，节点特征每迭代依次，就聚合了更高阶的邻节点的信息。随着 GCN 层数的增加，节点的聚合半径（最高邻居节点的阶数）也变大，一旦达到某个阈值，该节点覆盖全图节点。如果层数很多，每个节点覆盖的节点都会收敛到全图，这就导致每个节点的局部网络结构的多样性大大降低，对于节点自身特征的学习反而不好<sup><a href="#zhang2021evaluating">2</a></sup>。

当然，也有把深度做深的方法。DeepGCN<sup><a href="#li2019deepgcn">3</a></sup>借用CNN的概念，特别是residually/dense connections and dilated convolutions，将其适应GCN框架来实现。实验结果表明深度GCN框架的积极作用。最后，使用这些概念构建了一个非常深56层的GCN网络，显著提高了点云语义分割任务的性能。

### Q: 为什么有时候未训练的GNN反而效果更好？
确实如此。
> 通过与 Weisfeiler-Lehman 算法的类比，我们可以理解，即使是未经训练的具有随机权重的 GCN 模型也可以作为图中节点的强大特征提取器<sup><a href="#kipf2016semi">4</a></sup>。

## 参考文献
1. <p name="ijcai2017p250">Hamaguchi, T., Oiwa, H., Shimbo, M., &amp; Matsumoto, Y. (2017). <em>Knowledge Transfer for Out-of-Knowledge-Base Entities: A Graph Neural Network Approach.</em> In <em>Proceedings of the Twenty-Sixth International Joint Conference on Artificial Intelligence (IJCAI-17)</em> (pp. 1802-1808). doi: <a href="https://doi.org/10.24963/ijcai.2017/250">10.24963/ijcai.2017/250</a>. Available at <a href="https://doi.org/10.24963/ijcai.2017/250">https://doi.org/10.24963/ijcai.2017/250</a></p>
2. <p name="zhang2021evaluating">Zhang, W., Sheng, Z., Jiang, Y., Xia, Y., Gao, J., Yang, Z., &amp; Cui, B. (2021). <em>Evaluating Deep Graph Neural Networks.</em> <em>arXiv preprint arXiv:2108.00955.</em></p>
3. <p name="li2019deepgcn">Li, G., Muller, M., Thabet, A., &amp; Ghanem, B. (2019). <em>DeepGCNs: Can GCNs Go as Deep as CNNs?</em> In <em>Proceedings of the IEEE/CVF International Conference on Computer Vision</em> (pp. 9267-9276).</p>
4. <p name="kipf2016semi">Kipf, T. N., &amp; Welling, M. (2016). <em>Semi-supervised classification with graph convolutional networks.</em> <em>arXiv preprint arXiv:1609.02907</em>.</p>
5. <p name="velivckovic2017graph">Veličković, P., Cucurull, G., Casanova, A., Romero, A., Lio, P., &amp; Bengio, Y. (2017). <em>Graph attention networks.</em> <em>arXiv preprint arXiv:1710.10903</em>.</p>
6. <p name="wang2019heterogeneous">Wang, X., Ji, H., Shi, C., Wang, B., Ye, Y., Cui, P., &amp; Yu, P. S. (2019). <em>Heterogeneous graph attention network.</em> In <em>The World Wide Web Conference</em> (pp. 2022-2032).</p>






