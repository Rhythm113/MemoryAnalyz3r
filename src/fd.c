#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

int getfd(char *pidf) {

    int pid = atoi(pidf);
    char path[256];
    sprintf(path, "/proc/%d/fd", pid);

    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return 1;
    }

    struct dirent *entry;
    printf("Open file descriptors of process %d:\n", pid);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_LNK) {
            char buf[256];
            sprintf(buf, "%s/%s", path, entry->d_name);
            char target[256];
            ssize_t len = readlink(buf, target, sizeof(target) - 1);
            if (len < 0) {
                perror("readlink");
                return 1;
            }
            target[len] = '\0';
            printf("%s -> %s\n", entry->d_name, target);
        }
    }

    closedir(dir);

    return 0;
}
