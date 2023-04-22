## Xv6-Syscall
### 环境搭建
根据实验要求，xv6代码应当置于当前目录的src目录下。如果你还没有下载xv6，可以执行
```
./setting.sh
```
来完成环境配置
### 生成代码或一键通关
通过执行
```
sudo bash ./gen-ans.sh
```
来生成本题要求的代码，该文件会自动编辑对应的文件并运行测试程序。
### Notice
如果运行测试脚本时提示文件权限不够，请调用`bash ./setting.sh -c`。更多选项调用`bash ./setting.sh -h`以查看。
### setting.sh
快捷地操纵环境。如果没有任何参数，将安装软件包和xv6并运行
    使用方法：
    setting.sh [options][-b|-rc backup]
    options:
    -rs, --reset           重新安装xv6
    -rc, --recover         从参数中恢复xv6，默认为.backup
    -b, --backup           备份当前xv6至backuo参数，默认为.backup
    -c, --chmod-all        赋予所有*.sh和*.exp权限
    -np, --no-package      不安装软件包，只安装xv6
    -h, --help             显示帮助信息";

