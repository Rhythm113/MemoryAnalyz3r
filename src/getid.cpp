#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <proc/readproc.h>
#include"memory.h"

#define PATH_MAX 4096;

typedef struct _THREAD_DATA {
    pid_t threadId;
    char functionName[PATH_MAX];
} THREAD_DATA;

int getid(char* pido) {

    pid_t pid = atoi(pido);
    if (pid == 0) {
        fprintf(stderr, "Invalid PID: %s\n", pido);
        return EXIT_FAILURE;
    }

    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%d/task", pid);

    DIR* dir = opendir(path);
    if (dir == NULL) {
        fprintf(stderr, "Unable to open directory %s: %s\n", path, strerror(errno));
        return EXIT_FAILURE;
    }

    THREAD_DATA threadData[1024];
    int numThreads = 0;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }

        pid_t tid = atoi(entry->d_name);
        if (tid == 0) {
            fprintf(stderr, "Invalid TID: %s\n", entry->d_name);
            continue;
        }

        snprintf(path, PATH_MAX, "/proc/%d/task/%d/comm", pid, tid);

        int fd = open(path, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "Unable to open file %s: %s\n", path, strerror(errno));
            continue;
        }

        ssize_t count = read(fd, threadData[numThreads].functionName, PATH_MAX);
        if (count == -1) {
            fprintf(stderr, "Unable to read file %s: %s\n", path, strerror(errno));
            close(fd);
            continue;
        }

        close(fd);

        threadData[numThreads].functionName[count - 1] = '\0'; // Remove newline
        threadData[numThreads].threadId = tid;
        numThreads++;
    }

    closedir(dir);

    // Sort threads by thread ID
    qsort(threadData, numThreads, sizeof(THREAD_DATA), [](const void* a, const void* b) -> int {
        const THREAD_DATA* dataA = (const THREAD_DATA*)a;
        const THREAD_DATA* dataB = (const THREAD_DATA*)b;
        return (int)(dataA->threadId - dataB->threadId);
    });

    // Print out the sorted thread IDs and function names
    for (int i = 0; i < numThreads; i++) {
        printf("Thread ID: %d, Function: %s\n", threadData[i].threadId, threadData[i].functionName);
    }

    return EXIT_SUCCESS;
}



// gcc -o sort_functions sort_functions.c -lproc

