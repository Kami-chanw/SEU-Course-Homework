## Xv6-Threads
通过`gen_ans`来生成代码，测试文件为testthreads.c。当Xv6运行后，可以直接在xv6 shell中使用`testthreads`查看结果。
在这个实验中实现了要求的clone, join, thread_create, ticket lock和相关函数。需要注意的是，ticket lock似乎还有一些bug，时不时会有死锁。
