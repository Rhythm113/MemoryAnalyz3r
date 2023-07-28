#include <stdio.h>
#include <stdlib.h>
#include"memory.h"

int linfo(char *pid) {

    char command[1024];
    //libname removal & error parsing
   // sprintf(command, "ldd /proc/%s/exe | grep %s", pid, libname);
   sprintf(command, "ldd /proc/%s/exe || echo 'ldd not found install by: pkg install ldd'", pid);

    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("command failed");
        exit(EXIT_FAILURE);
    }

    char output[1024];
    while (fgets(output, sizeof(output), fp) != NULL) {
        printf("%s", output);
    }

    pclose(fp);

    return 0;
}
