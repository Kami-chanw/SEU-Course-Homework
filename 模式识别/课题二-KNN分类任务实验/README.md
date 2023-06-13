## 文件说明
本实验有四个任务，其中每一个任务都要求把对测试集的预测输出到文件`task*_test_prediction.csv`中。
任务和文件的对应关系如下：
- 任务一：要求手动实现KNN，`utils.py`包含了KNN的实现,`train.py`包含了使用、绘图的代码。
- 任务二：要求证明一个公式（实际上是NCA）并以梯度下降法训练，证明过程见实验报告，代码位于`train_nca.py`，这一任务不保证正确。
- 任务三：要求对比MindSpore（但MindSpore没有机器学习算法，因此对比的是sklearn），`trian_sklearn.py`包含了这一部分代码。
- 任务四：要求在其他数据集上对比自己实现的算法和官方代码，我在breast cancer上完成了这个任务，代码位于`train_breast_cancer.py`