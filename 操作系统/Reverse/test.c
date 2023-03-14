﻿#include "dlist.h"
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUFFER 1024
FILE* read_file(const char* path, const char* mode) {
    FILE* fp = fopen(path, mode);
    if (fp == NULL) {
        fprintf(stderr, "reverse: cannot open file \'%s\'\n", path);
        exit(EXIT_FAILURE);
    }
    return fp;
}

FILE * src, *des;
list_t list;
void   argparse(int argc, char** argv) {
    switch (argc) {
    case 1:
        src = stdin;
        des = stdout;
        break;
    case 2:
        src = read_file(argv[1], "r");
        des = stdout;
        break;
    case 3:

        src                = read_file(argv[1], "r");
        des                = read_file(argv[2], "w");
        int         fd_src = fileno(src), fd_des = fileno(des);
        struct stat st_src, st_des;
        fstat(fd_src, &st_src);
        fstat(fd_des, &st_des);
        if (st_src.st_ino == st_des.st_ino || strcmp(argv[1], argv[2]) == 0) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(EXIT_FAILURE);
        }

        break;
    default:
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv) {
    argparse(argc, argv);
    init_list(&list);

    size_t len      = 0;
    size_t capacity = MAX_BUFFER;
    char   buffer[MAX_BUFFER], *ptr = buffer;

    for (char ch; (ch = fgetc(src)) && !feof(src); ptr = buffer, len = 0) {
        for (; ch != '\n'; ch = fgetc(src)) {
            ptr[len++] = ch;
            if (len == MAX_BUFFER) {
                char* newbuf = (char*)allocate(capacity = capacity + capacity / 2);
                memcpy(newbuf, ptr, MAX_BUFFER);
            }
            else if (len == capacity) {
                if ((ptr = realloc(ptr, capacity = capacity + capacity / 2)) == NULL) {
                    fprintf(stderr, "malloc failed\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
        if (len) {
            ptr[len]   = '\0';
            char* data = (char*)allocate(len);
            memcpy(data, ptr, len + 1);
            push_front(&list, data);
        }
    }

    for (node_t* cur = list.head; cur; cur = cur->next) {
        fprintf(des, "%s\n", cur->data);
    }
    destroy_list(&list);
}