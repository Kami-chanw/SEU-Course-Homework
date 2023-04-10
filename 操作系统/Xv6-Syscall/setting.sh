#! /bin/bash

install_xv6() {
    rm -rf src backup.tgz
    git clone https://github.com/mit-pdos/xv6-public.git
    if [ $? == 0 ]; then
        mv xv6-public src
        tar -czf backup.tgz src
        cd ./src
        make
    fi
}
check_xv6(){
    if ! [ -d src ]; then
        echo "尚未安装xv6";
        exit 1;
    fi
}
chmod_sh() {
    if [[ ${PWD##*/} == "Xv6-Syscall" && -d "../tester" ]]; then
        chmod 777 ./*.sh ../tester/*
    else
        echo "未在父目录找到tester目录或未置于'Xv6-Syscall/'目录下"
        exit 1
    fi
    exit 0;
}
version_ctrl() {
    check_xv6;
    if [ -z "$2" ]; then
        dir="backup.tgz"
    else
        dir="$2"
    fi
    if [ "$1" = "backup" ]; then
        tar -czf $dir src
    elif [ "$1" = "recover" ]; then
        if ! [ -f "$dir" ]; then
            echo "没有找到备份文件"
            exit 1;
        else
            rm -rf src/*
            tar -xzf $dir
        fi
    else
        echo "错误的version_ctrl参数"
        exit 1
    fi
    exit 0
}
while [[ $# -gt 0 ]]; do
    case "$1" in
        --chmod-all | -c)
            chmod_sh;;
        --recover | -rc)
            version_ctrl recover $2;;
        --backup | -b)
            version_ctrl backup $2;;
        --reset | -rs | --no-package | -np)
            install_xv6;
            exit 0 ;;
        --help | -h) echo "
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
            exit 0;;
        *) echo "未知选项: $1" >&2; exit 1 ;;
    esac
    shift
done
sudo apt install qemu git build-essential tcl tk expect
if [ $? == 0 ]; then
    install_xv6
    make qemu
fi

