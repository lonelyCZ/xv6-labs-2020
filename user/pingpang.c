#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int p1[2];
    int p2[2];
    char buf;

    pipe(p1);
    pipe(p2);
    if(fork() == 0) {
        close(p1[1]);
        close(p2[2]);
        read(p1[0], &buf, 1);
        printf("%d: received ping\n", getpid());
        write(p2[1], "1", 1);
    } else {
        close(p1[0]);
        close(p2[1]);
        write(p1[1], "1", 1);
        read(p2[0], &buf, 1);
        printf("%d: received pong\n", getpid());
    }

    exit(0);
}