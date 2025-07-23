#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    int p2c[2];
    int c2p[2];
    if (pipe(p2c) == -1 || pipe(c2p) == -1) {
        fprintf(2,"failed to create pipe");
        exit(1);
    }
    char buf = 'x';
    if (fork() == 0) {
        close(c2p[0]);  // close read end of c2p
        close(p2c[1]);  // close write end of p2c
        if (read(p2c[0], &buf, 1) == 1) {    // wait to receive parent
            printf("%d: received ping\n", getpid());
        } else {
            fprintf(2, "child failed to receive ping from parent\n");
            exit(1);
        }
        if (write(c2p[1], &buf, 1) != 1) {
            fprintf(2, "child failed to send pong to parent\n");
            exit(1);
        }
        exit(0);
    } else {
        close(p2c[0]);  // close read end of p2c
        close(c2p[1]);  // close write end of c2p
        if (write(p2c[1], &buf, 1) != 1) {
            fprintf(2, "parent failed to send ping to child\n");
            exit(1);
        }
        if (read(c2p[0], &buf, 1) == 1) {    // wait to receive from child
            printf("%d: received pong\n", getpid());
        } else {
            fprintf(2, "parent failed to receive pong from child\n");
            exit(1);
        }
        wait(0);
        exit(0);
    }
}