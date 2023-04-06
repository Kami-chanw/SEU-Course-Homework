#! /bin/bash

install=true
install_xv6() {
    rm -rf src .backup
    git clone https://github.com/mit-pdos/xv6-public.git
    if [ $? == 0 ]; then
        mv xv6-public src
        mkdir .backup
        cp -r src/* .backup
        cd ./src
        make && make qemu
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
}
while [[ $# -gt 0 ]]; do
    case "$1" in
        --no-install | -ni) 
            install=false ;;
        --chmod-all | -c)
            chmod_sh;
            exit 0;;
        --recover | -rc)
            check_xv6;
            if ! [ -d .backup ]; then
                echo "没有备份目录"
                exit 1;
            else
                rm -rf src/*
                cp -r .backup/* src
                exit 0;
            fi
            ;;
        --backup | -b)
            check_xv6;
            if ! [ -d .backup ]; then
                mkdir .backup;
            else
                rm -rf .bakcup/*
            fi
            cp -r src/* .backup
            exit 0;
            ;;
        --reset | -rs) 
            check_xv6;
            install=false ;;
        --help | -h) echo "使用方法：
    -rs, --reset           重新安装xv6
    -rc, --recover         使用备份覆盖当前xv6
    -b, --backup           备份当前xv6
    -c, --chmod-all        赋予所有*.sh和*.exp权限
    -ni, --no-install      不安装软件包，只安装xv6
    -h, --help             显示帮助信息";
            exit 0;;
        *) echo "未知选项: $1" >&2; exit 1 ;;
    esac
    shift
done
if $install; then
    sudo apt install qemu git build-essential tcl tk expect
    if [ $? == 0 ]; then
        install_xv6
    fi
else
    install_xv6
fi
