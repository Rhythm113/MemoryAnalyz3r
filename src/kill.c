#include "memory.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

int kill(int tid)
{

    // Parse the PID and TID arguments
   // pid_t pid = atoi(argv[1])
    //pid_t tid = tid;

    // Send a signal to the target thread to terminate it
    if (kill(tid, SIGKILL) < 0)
    {
        printf("Failed to kill thread\n");
        return 1;
    }

    return 0;
}