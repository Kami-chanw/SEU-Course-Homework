if [[ ! -d "./src" ]]; then
    bash ./setting.sh
fi
sed -i "/yield(void)/a\int getreadcount(void);\n" ./src/defs.h
sed -i -e "/sys_uptime(void)/a\extern int sys_getreadcount(void);\n\extern int read_count;" \
    -e "/sys_close,/a\[SYS_getreadcount] sys_getreadcount," \
    -e "/tf->eax;/a\ \tif(num == SYS_read)\n\t\ read_count++;" ./src/syscall.c
echo "#define SYS_getreadcount 22" >> ./src/syscall.h
echo "SYSCALL(getreadcount)" >> ./src/usys.S
sed -i "/uptime(void)/a\int getreadcount(void);" ./src/user.h
echo -e "$a\nint read_count = 0;\nint sys_getreadcount(void) {\n\treturn read_count;\n}" >> ./src/sysproc.c
bash ./test-getreadcount.sh