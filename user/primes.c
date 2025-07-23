#include "kernel/types.h"
#include "user/user.h"

void primes(int p[2]) {
    int newp[2];
    pipe(newp);
    int prime;
    if (read(p[0], &prime, 4) == 0) return;
    printf("prime %d\n", prime);
    int n;
    while(read(p[0], &n, 4) == 4) {
        if (n % prime) write(newp[1], &n, 4);
    }
    close(p[0]); close(newp[1]);
    if (fork() == 0) {
        primes(newp);
    } else {
        wait(0);
    }
    exit(0);
}


int main(int argc, char* argv[]) {
    int p[2];
    pipe(p);

    for (int i = 2; i <= 35; i++) {
        if (write(p[1], &i, 4) != 4) {
            fprintf(2, "initial pipe failed\n");
        }
    }
    close(p[1]);

    int newp[2];
    pipe(newp);
    int prime;
    if (read(p[0], &prime, 4) != 4) {
        fprintf(2, "failed to read from the first pipe\n");
    }
    printf("prime %d\n", prime);
    int n;
    while(read(p[0], &n, 4) == 4) {
        if (n % prime) write(newp[1], &n, 4);
    }
    close(p[0]); close(newp[1]);
    if (fork() == 0) {
        primes(newp);
    } else {
        wait(0);
    }
    exit(0);
}
