#include <stdio.h>
#include <stdint.h>
#include<iostream>
#include <algorithm>
#include<cstdlib>
#include<string>
#include <fstream>
#include<sys/types.h>
#include<unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <ctype.h>
//#include"ob.h"
using namespace std;

#include"memory.h"

void kill_thread(int tid) {
    printf("Killing thread %d...\n", tid);
    kill(tid);
}

void open_support_chat() {
    // open support chat URL in browser
    system("/data/data/com.termux/files/usr/bin/termux-open-url https://t.me/InfinityCreatorsChat");
}


int getPID(char * PackageName)
{
	DIR *dir = NULL;
	struct dirent *ptr = NULL;
	FILE *fp = NULL;
	char filepath[256];
	char filetext[128];
	dir = opendir("/proc");
	if (NULL != dir)
	{
		while ((ptr = readdir(dir)) != NULL)
		{

			if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
				continue;
			if (ptr->d_type != DT_DIR)
				continue;
			sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);
			fp = fopen(filepath, "r");
			if (NULL != fp)
			{
				fgets(filetext, sizeof(filetext), fp);
				if (strcmp(filetext, PackageName) == 0)
				{

					break;
				}
				fclose(fp);
			}
		}
	}
	if (readdir(dir) == NULL)
	{
		return 0;
	}
	closedir(dir);
	return atoi(ptr->d_name);
}

// print program usage
void print_usage() {
    printf("Memory Analyz3r by @Rhythm113\n\n");
    printf("Usage: mem [options] [PACKAGE NAME/TID]\n\n");
    printf("Basic Commands:\n");
    printf("-f, --fd-read         Reads file descriptors on a process, needs package name as input\n");
    printf("-t, --tid-data        Prints TID information\n");
    printf("-s, --symbol-runtime  Prints runtime Symbols\n");
    printf("-l, --linker-details  Prints linked addresses\n");
    printf("-k, --kill            Kills a thread by TID. Needs tid as input\n");
    printf("--support             Goes to support chat\n\n");
    printf("Other Commands:\n");
    printf("-c, --compiler      Compiles a python script to c shared Library. Use -c or --compiler then the python file directory\n");
    printf("-h, --help            Prints this help message\n");
    printf("--version             Prints program version\n");
}

// print program version
void print_version() {
    printf("Version: 0.4 (Early Access)\n");
}

// main function
int main(int argc, char *argv[]) {

    // define long options
    struct option long_options[] = {
        {"fd-read",         required_argument, 0, 'f'},
        {"tid-data",        required_argument, 0, 't'},
        {"symbol-runtime",  required_argument, 0, 's'},
        {"linker-details",  required_argument, 0, 'l'},
        {"kill",            required_argument, 0, 'k'},
        {"compiler",       required_argument,  0, 'c'},
        {"help",            no_argument,       0, 'h'},
        {"version",         no_argument,       0, 'v'},
        {"support",         no_argument,       0, 'x'},
        {0, 0, 0, 0}
    };

    // parse command line arguments
    int opt;
    bool option_selected = false;
    char pid[16];
    while ((opt = getopt_long(argc, argv, "f:t:s:l:k:c:hvx", long_options, NULL)) != -1) {
        switch (opt) {
            case 'f':
            option_selected = true;
                sprintf(pid,"%d", getPID(argv[2]));
                getfd(pid);
                break;
            case 't':
            option_selected = true;
                sprintf(pid,"%d", getPID(argv[2]));
                getid(pid);
                break;
            case 's':
            option_selected = true;
                sprintf(pid,"/proc/%d/exe", getPID(argv[2]));
                getsym(pid);
                break;
            case 'l':
            option_selected = true;
                sprintf(pid,"%d", getPID(argv[2]));
                linfo(pid);
                break;
            case 'k':
            option_selected = true;
                kill(atoi(argv[2]));
                break;
            case 'h':
            option_selected = true;
                print_usage();
                return 0;
            case 'v':
            option_selected = true;
                print_version();
                return 0;
            case 'x':
            option_selected = true;
                printf("Going to support chat...\n");
                open_support_chat();
                return 0;
            case 'c':
            option_selected = true;
                compile(argv[2]);
                return 0;
                
            default: // invalid option
                print_usage();
                return 1;
        }
    }


      if (!option_selected) {
        print_usage();
        return 1;
    }

    return 0;
    
}

