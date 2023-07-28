#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sysinfo.h>

int pinfo(char *pido) {
    int pid = atoi(pido);
    struct sysinfo meminfo;
    char path[256];
    sprintf(path, "/proc/%d/statm", pid);

    if (sysinfo(&meminfo) < 0) {
        perror("sysinfo");
        return 1;
    }

    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    unsigned long size, resident, shared, text, lib, data, dt;
    fscanf(fp, "%lu %lu %lu %lu %lu %lu %lu", &size, &resident, &shared, &text, &lib, &data, &dt);
    fclose(fp);

    printf("Memory usage of process %d:\n", pid);
    printf("Total program size: %lu KB\n", size);
    printf("Resident set size: %lu KB\n", resident);
    printf("Shared pages: %lu KB\n", shared);
    printf("Code segment size: %lu KB\n", text);
    printf("Shared library size: %lu KB\n", lib);
    printf("Data segment size: %lu KB\n", data);
    printf("Dirty pages size: %lu KB\n", dt);
    printf("Total system memory: %lu KB\n", meminfo.totalram / 1024);
    printf("Free memory: %lu KB\n", meminfo.freeram / 1024);

    return 0;
}
