#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char* argv[]) {
    int argcount = 0;
    char block[32]; // 按块读取
    char curarg[95]; // 参数
    char* p = curarg; // 下一个参数的起始位置
    char* lineSplit[MAXARG];  // MAXARG为32定义在kernel/param.h中
    for (int i = 1; i < argc; i++) {
        lineSplit[argcount] = argv[i]; // xarg后的命令
        argcount++;
    }
    int k, m = 0;
    while ((k = read(0, block, sizeof(block))) > 0 ) {
        for (int j = 0; j < k; j++) {
            if (block[j] == '\n') {
                curarg[m] = 0; // 字符串补0
                lineSplit[argcount] = p;
                argcount++;
                lineSplit[argcount] = 0; // 参数组最后补0
                if (fork() == 0) {
                    exec(argv[1], lineSplit);
                }
                wait(0);
                m = 0;
                p = curarg;
                argcount = argc - 1;
            }
            else if (block[j] == ' ') {
                curarg[m] = 0;
                m++;
                lineSplit[argcount] = p;
                argcount++;
                p = &curarg[m];
            }
            else {
                curarg[m] = block[j];
                m++;
            }
        }
    }
    exit(0);
}