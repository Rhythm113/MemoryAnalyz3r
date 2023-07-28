#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_PATH_LENGTH 4096

int fileExists(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file != NULL) {
        fclose(file);
        return 1;  // File exists
    }
    return 0;  // File does not exist
}

void create_output_folder() {
    const char* folder_name = "output";

    // Check if the folder already exists
    struct stat st;
    if (stat(folder_name, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            printf("Output folder already exists.\n");
            return;
        }
    }

    // Create the output folder
    if (mkdir(folder_name, 0700) == 0) {
        printf("Output folder created successfully.\n");
    } else {
        printf("Failed to create output folder.\n");
        exit(1);
    }
}

int compile(char *path) {
    
    int v = fileExists(path);
    if(!v){
    printf("Input file doesn't exists\n");
    exit(-1);
    }

    // Extract the file name from the path
    //char *path = "";
    char *filename = strrchr(path, '/');
    if (filename == NULL) {
        filename = path;
    } else {
        filename++;  // Skip the '/' character
    }

    // Print the present working directory
    char cwd[MAX_PATH_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Present Working Directory: %s\n", cwd);
    } else {
        printf("Failed to get the present working directory.\n");
        exit(1);
    }

    // Run the 'cython' command
    printf("Generating C++ file.\n");
    char cythonCommand[MAX_PATH_LENGTH];
    snprintf(cythonCommand, sizeof(cythonCommand), "/data/data/com.termux/files/usr/bin/cython --embed -o %s.c %s > /dev/null 2>&1", filename, path);
    if (system(cythonCommand) != 0) {
        printf("Failed to run the command. Is required lib installed? or the file present?\n");
        exit(1);
    }

    // Modify the generated C file to include the Cython directive for Python 3
    char modifyCommand[MAX_PATH_LENGTH];
    snprintf(modifyCommand, sizeof(modifyCommand), "sed -i '1s/^/#define PY_MAJOR_VERSION 3\\n/' %s.c > /dev/null 2>&1", filename);
    if (system(modifyCommand) != 0) {
        printf("Failed to modify the generated C file.\n");
        exit(1);
    }

    // Run the 'g++' command
    printf("Building Library...\n");
    char gccCommand[MAX_PATH_LENGTH];
    snprintf(gccCommand, sizeof(gccCommand), "/data/data/com.termux/files/usr/bin/g++ -shared -o %s.so -fPIC $(python3-config --cflags) $(python3-config --ldflags) %s.c > /dev/null 2>&1", filename, filename);
    if (system(gccCommand) != 0) {
        printf("Failed to run the 'g++' command.\n");
        exit(1);
    }

    // Create the output folder
    create_output_folder();

    // Remove the 'fn.c' file
    char removeCommand[MAX_PATH_LENGTH];
    snprintf(removeCommand, sizeof(removeCommand), "rm %s.c > /dev/null 2>&1", filename);
    if (system(removeCommand) != 0) {
        printf("Failed to remove the C file.\n");
        exit(1);
    }

    // Move 'fn.so' to the output folder
    char moveCommand[MAX_PATH_LENGTH];
    snprintf(moveCommand, sizeof(moveCommand), "mv %s.so output/%s.so > /dev/null 2>&1", filename, filename);
    if (system(moveCommand) != 0) {
        printf("Failed to move the shared object file.\n");
        exit(1);
    }

    // Echo the output path
    char outputFilePath[MAX_PATH_LENGTH];
    snprintf(outputFilePath, sizeof(outputFilePath), "%s/output/%s.so", cwd, filename);
    printf("Output saved in: %s\n", outputFilePath);

    return 0;
}
