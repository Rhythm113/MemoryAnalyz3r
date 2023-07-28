# MemoryAnalyz3r
Simple Utility to scan specific section of memory.<br> Mainly Built & Tsted for Android Arm64

# Usage : 
```
Memory Analyz3r by @Rhythm113

Usage: mem [options] [PACKAGE NAME/TID]

Basic Commands:
-f, --fd-read         Reads file descriptors on a process, needs package name as input
-t, --tid-data        Prints TID information
-s, --symbol-runtime  Prints runtime Symbols
-l, --linker-details  Prints linked addresses
-k, --kill            Kills a thread by TID. Needs tid as input
--support             Goes to support chat

Other Commands:
-h, --help            Prints this help message
--version             Prints program version
```

# Compiling : 
`g++ *.c *.cpp -o mem -lelf -lproc `
