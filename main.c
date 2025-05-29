#include "eleg.h" 
#include "parsearg.h"

int 
main(int argc, char **argv){
    argv = ParseArgList(&argc, argv);

    FileStatus *file_status_list = FileStatusListInit(FILE_LIST, FILE_COUNT); 

    // Now, we make the children.
    for (ssize_t i = 0; i < FILE_COUNT; ++i) {
        pid_t fork_val = fork();
        ERR_HANDLE(fork_val == -1,
                   "ERROR: could not fork process; errno: %d",
                   exit(errno),
                   errno);
        if (fork_val == 0) {
            // Yes, this code is correct. 
            exit(file_status_list[i].status =
                 FileSearch(FILE_LIST[i], TARGET_WORD));
        }
    }

    // This is so that the parent doesn't act while a child process is still
    // running. The status list analysis begins only after all the children
    // terminate. Otherwise, wrong output is generated.
    while (wait(NULL) >= 0);

    FileShowStatusList(TARGET_WORD, file_status_list, FILE_COUNT);

    CLEAN_UP(argv, file_status_list);

    return EXIT_SUCCESS;
}
