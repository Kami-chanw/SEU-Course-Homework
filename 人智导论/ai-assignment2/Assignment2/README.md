## 人工智能课程作业2 -- 约束满足问题

#### 简介

本作业基于Eight Queen Puzzle，目标是基于回溯搜索（`backtrackSearch`）和最小冲突搜索（`minConflict`）算法求该问题的解。该部分内容对应《Artificial Intelligence: A Modern Approach 3rd》中的第六章内容：Constraint Satisfaction Problems。

##### Eight Queen Puzzle

该问题为一个8*8的棋盘，在该棋盘上摆放8个皇后，使其不能互相攻击，即任意两个皇后都不能处于同一行、同一列或同一斜线（主对角线、次对角线）上。

有多种摆放方案可以满足上述条件，例如

```shell
0 1 0 0 0 0 0 0
0 0 0 0 0 1 0 0
1 0 0 0 0 0 0 0
0 0 0 0 0 0 1 0
0 0 0 1 0 0 0 0
0 0 0 0 0 0 0 1
0 0 1 0 0 0 0 0
0 0 0 0 1 0 0 0
```



#### 作业目标

完成Assignement2项目中的五个函数

search.cpp中

```c++
std::vector<Queen*> backtrack(std::vector<Queen*> assignment, Csp& csp);
std::vector<Queen*> minConflict(Csp& csp, int maxSteps);
int getConflicts(Csp& csp, Position& position);
Queen* chooseConflictVariable(Csp& csp);
Position getMinConflictValue(Csp& csp, Queen* var);
```

其中`minConflict`函数依赖于最后三个。

##### 项目启动说明

使用Visual Studio 2019打开Assignment2.sln。

##### Tips

- `main.cpp`中的`main`函数为该项目的主入口。



#### 评估标准

##### 源代码部分（8分）

1. 搜索算法的正确性：算法得出的结果可以通过`main.cpp`中`searchTest`函数的验证。
2. 搜索算法的搜索时间
   - 回溯搜索可以在5秒内跑出puzzle的结果
   - 最小冲突搜索大部分可以在200步之内得出结果。
3. 内存管理：算法的内存消耗需要符合该算法应有的空间复杂度。
4. 回溯搜索（第一个函数）和最小冲突（后四个函数）各占4分。

##### 文档部分（2分）

1. 算法、实验结果分析：根据实验结果分析、对比回溯搜索和最小冲突搜索，完成“人工智能-第二次课程作业报告”。（1分）
2. 代码风格。(1分)



#### 作业提交

##### 提交截止时间

第11周上课时间。

##### 提交方式

将`search.cpp`和`作业报告文档`打包为zip文件，命名为`学号_姓名_Assignment2`，发送至邮箱`chuanqi_shi@163.com`。