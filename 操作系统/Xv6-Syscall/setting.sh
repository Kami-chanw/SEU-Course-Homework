install=true
install_xv6() {
    git clone https://github.com/mit-pdos/xv6-public.git
    mv xv6-public src
    cd ./src
    make && make qemu
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
        --reset | -r) 
            rm -rf src;
            install=false ;;
        --help | -h) echo "使用方法：
    -r, --reset            重新安装xv6
    -c, --chmod-all        赋予所有*.sh和*.exp权限
    -ni, --no-install      不安装软件包
    -h, --help             显示帮助信息";
            exit 0;;
        *) echo "未知选项: $1" >&2; exit 1 ;;
    esac
    shift
done
if $install; then
    sudo apt install qemu git build-essential tcl tk expect | sudo tee -a log.txt
    if ! grep -qE 'Abort|中止|^C' log.txt; then
        install_xv6
    fi
    rm -f log.txt
else
    install_xv6
fi
