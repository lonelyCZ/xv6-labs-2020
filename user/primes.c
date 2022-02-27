#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void func(int size, int* p1) {
    int p2[2];
    int counter = 0;
    char buffer[4];
    int nums[34];
    int prime;
    pipe(p2);
    read(p1[0], buffer, 4);
    prime = *((int*)buffer);
    printf("prime %d\n", prime);
    if (size == 1) {
        return;
    }
    size--;
    for (int i = 0; i < size; i++) {
        read(p1[0], buffer, 4);
        int temp = *((int*)buffer);
        if (temp % prime != 0) {
            nums[counter] = temp;
            counter++;
        }
    }
    close(p1[0]);
    for (int j = 0; j < counter; j++) {
        write(p2[1], (char*)(nums + j), 4);  // 将不能被当前质数整除的数传到下一步中处理
    }
    if (fork() == 0) {
        func(counter, p2);
        exit(0);
    }
    wait(0);
}

int main()
{
    int p[2];
    pipe(p);
    for (int i = 0; i < 34;  i++) {
        int temp = i + 2;
        write(p[1], (char*)(&temp), 4);  // 第一个管道放入2-35
    }
    if (fork() == 0) {
        func(34, p); // 传入2-35
    }
    wait(0); // 等待子程序退出

    exit(0);
}