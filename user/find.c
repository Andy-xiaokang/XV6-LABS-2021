
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* filename(char* path) {
    char* p;
    for (p = path + strlen(path); p >= path && *p != '/'; p--);
    p++;
    return p;
}

void find(char *path, char *fname) {
    char buf[512], *p;
    int fd;
    struct stat st;
    struct dirent de;
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0) {
        fprintf(2, "cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch(st.type) {
        case T_FILE:    // base case
            if (strcmp(filename(path), fname) == 0) printf("%s\n", path);   // base case
            break;
        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
                printf("path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                find(buf, fname);
            }
            break;
    }
    close(fd);
    return;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(2, "argument number error");
    }
    find(argv[1], argv[2]);
    exit(0);
}