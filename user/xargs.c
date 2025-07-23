#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int readline(char** argv, int argc) {
    char* p = argv[argc];
    int n;
    while((n = read(0, p, 1)) == 1) {
        if (*p == '\n') {
            *p = 0;
            return 1;
        } else {
            p++;
        }
    }
    if (n == 0) {   // EOF
        return 0;
    }
    if (n < 0) {
        fprintf(2, "readline error");
        return 0;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    int new_argc = argc - 1;
    char* new_argv[MAXARG]; // 未初始化的指针不可以用 strcpy 把内容复制到一个未分配内存的位置
    for (int i = 1; i < argc; i++) {
        new_argv[i-1] = malloc(strlen(argv[i] + 1));
        strcpy(new_argv[i-1], argv[i]);
    }
    new_argv[new_argc] = malloc(1024);
    new_argv[new_argc+1] = 0;
    while (readline(new_argv, new_argc)) {
        if (fork() == 0) {
            exec(new_argv[0], new_argv);
        } else {
            wait(0);
        }
    }
    for (int i = 0; i <= new_argc; i++) {
        free(new_argv[i]);
    }
    exit(0);
}