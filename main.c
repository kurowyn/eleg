#include "utils.h"

int main(int argc, char **argv){
    int file_count = argc - 2;

    int *file_statuses = mmap(NULL, 
                              file_count * sizeof(int),
                              PROT_READ | PROT_WRITE, 
                              MAP_SHARED | MAP_ANONYMOUS,
                              -1,
                              0);

    for (int i = 0; i < file_count; ++i)
        file_statuses[i] = -1;

    char **files = argv + 2;

    char *word = argv[1];

    if (argc < 3) {
        fprintf(stderr, "usage: ./a.out (word) (FILE(s))");
        exit(EXIT_FAILURE);
    }

    // Now, we make the processes.

    for (size_t i = 0; i < file_count; ++i) {
        int fork_val = fork();
        ERR_HANDLE(fork_val == -1,
                   "ERROR: could not fork process; errno: %d",
                   exit(errno),
                   errno);
        if (fork_val == 0) {
            // The search_file function now returns an exit code,
            // instead of the function(s) itself (themselves) exiting.
            // This is still safe.
            file_statuses[i] = search_file(files[i], word);
            exit(file_statuses[i]);
        } 
    }

    for (size_t i = 0; i < file_count; ++i) {
        int child_status;
        wait(&child_status);

        if (WIFEXITED(child_status)) {
            switch (file_statuses[i]) {
                case WORD_FOUND:
                    printf("%d %s: %s found.\n",
                           i + 1, files[i], word);
                    break;
                case WORD_NOT_FOUND:
                    printf("%d %s: %s not found.\n",
                           i + 1, files[i], word);
                    break;
                case FILE_NOT_FOUND:
                    printf("%d %s: no such file.\n",
                           i + 1, files[i]);
                    break;
                default:
                    fprintf(stderr, 
                            "ERROR: unknown error occured (exit statuses: %d, %d).\n"
                            "Contact your system adminstrator.\n",
                            file_statuses[i], WEXITSTATUS(child_status));
                    break;
            }
        }
    }

    return EXIT_SUCCESS;
}
