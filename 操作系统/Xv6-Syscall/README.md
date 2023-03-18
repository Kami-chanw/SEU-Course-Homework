## Xv6-Systemcall
### 环境搭建
根据实验要求，xv6代码应当置于当前目录的src目录下。如果你还没有下载xv6，可以执行
```
sudo bash setting.sh
```
来完成环境配置
### 生成代码
通过执行
```
sudo bash gen-ans.sh
```
来生成本题要求的代码，该文件会自动编辑对应的文件。
### Notice
如果运行测试脚本时提示文件权限不够，请先进入Xv6-Syscall，然后调用以下代码
```
cd ..
find . -name "*.sh" -exec chmod +x {} \;
```
