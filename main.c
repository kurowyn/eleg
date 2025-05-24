#include "utils.h"

// Putting this in a different file can make things a little too abstracted,
// so it's here directly.
#define PROGRAM_NAME argv[0]
#define TARGET_WORD argv[1]
#define FILE_COUNT argc - 2
#define FILE_LIST argv + 2

int 
main(int argc, char **argv){
    if (argc < MIN_ARG_COUNT) {
        fprintf(stderr, "usage: %s (word) (FILE(s))", PROGRAM_NAME);
        exit(EXIT_FAILURE);
    }

    FileStatus *FileStatusList = FileStatusListInit(FILE_LIST, FILE_COUNT); 

    // Now, we make the processes.
    for (size_t i = 0; i < FILE_COUNT; ++i) {
        pid_t fork_val = fork();
        ERR_HANDLE(fork_val == -1,
                   "ERROR: could not fork process; errno: %d",
                   exit(errno),
                   errno);
        if (fork_val == 0) {
            // The search_file function now returns the exit code,
            // instead of the function(s) itself (themselves) exiting.
            // This is still safe.
            // Yes, this code is what it looks like.
            exit(FileStatusList[i].status = search_file(FileStatusList[i].name,
                                                        TARGET_WORD));
        }
    }

    // This is so that the parent doesn't act while a child process is still
    // running. The status list analysis begins only after all the children
    // terminate. Check the manpage for more info.
    while (wait(NULL) >= 0);

    for (size_t i = 0; i < FILE_COUNT; ++i) {
        switch (FileStatusList[i].status) {
            case WORD_FOUND:
                printf("%d %s: %s found.\n",
                       i + 1, FileStatusList[i].name, TARGET_WORD);
                break;
            case WORD_NOT_FOUND:
                printf("%d %s: %s not found.\n",
                       i + 1, FileStatusList[i].name, TARGET_WORD);
                break;
            case FILE_NOT_FOUND:
                printf("%d %s: no such file.\n",
                       i + 1, FileStatusList[i].name);
                break;
            default:
                // Under normal circumstances, this should not happen,
                // except if search_file() returns an unordinary exit code.
                // (which means, practically speaking, it shouldn't 
                // ever occur). 
                fprintf(stderr, "ERROR: unkown error occurred.\n"
                                "File status: %d, errno: %d.\n"
                                "Contact your system adminstrator.\n",
                                FileStatusList[i].status, errno);
                break;
        }
    }

    return EXIT_SUCCESS;
}
