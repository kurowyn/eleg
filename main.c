#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>

#define KURO_VERSION
//#define PROJECT_VERSION

#if defined(PROJECT_VERSION)
#define MAX_BUFFER_LEN 1024
#endif

#define ERR_HANDLE(err_cond, err_msg, err_code, ...) \
    if (err_cond) { \
        fprintf(stderr, err_msg, __VA_ARGS__); \
        exit(err_code); \
    } \

enum {WORD_FOUND = 0,
      WORD_NOT_FOUND = 1,
      FILE_NOT_FOUND = 2};

char *
dynamic_read(const char *filename){
    char *buffer = malloc(0);
    FILE *f = fopen(filename, "r");

    ERR_HANDLE(buffer == NULL, 
               "ERROR: could not allocate memory, errno: %d",
               EXIT_FAILURE,
               errno);

    ERR_HANDLE(f == NULL,
               "ERROR: could not open file %s, errno: %d\n",
               errno == ENOENT ? FILE_NOT_FOUND : errno,
               filename, errno);

    // ENOENT: file doesn't exit. (check manpage for errno)
    // This is to filter out the cases in which the file does exist,
    // but for some reason could not be opened. In that case, we exit
    // with the errno code.

    for (;;) {
        static int i = 0, c;
        if ((c = fgetc(f)) != EOF) {
            // This is for the unlikely event we encounter 
            // NULL characters in our file.
            if (c != '\0') {
                buffer = realloc(buffer, (++i) * sizeof (char));

                ERR_HANDLE(buffer == NULL, 
                           "ERROR: could not reallocate memory, errno: %d",
                           EXIT_FAILURE,
                           errno);

                buffer[i - 1] = c;
            } 
        } else {
                buffer[i - 1] = '\0';
                break;
        }
    }

    return buffer;
}

int 
search_file(const char *filename, const char *token){
#if defined(KURO_VERSION)
    char *buffer = dynamic_read(filename);

    int found = strstr(buffer, token) != NULL ? WORD_FOUND : WORD_NOT_FOUND;

    free(buffer);

    return found;
#elif defined(PROJECT_VERSION)
    FILE *f = fopen(filename, "r");

    ERR_HANDLE(f == NULL,
               "ERROR: could not open file %s, errno: %d\n",
               errno == ENOENT ? FILE_NOT_FOUND : errno,
               filename, errno);

    char buffer[MAX_BUFFER_LEN];

    while (fgets(buffer, MAX_BUFFER_LEN, f) != NULL) {
        if (strstr(buffer, token) == NULL) {
            continue;
        }
        return WORD_FOUND;
    }

    return WORD_NOT_FOUND;
#endif
}

int main(int argc, char **argv){
    int file_count = argc - 2;
    char *word = argv[1];

    if (argc < 3) {
        fprintf(stderr, "usage: ./a.out (word) (FILE(s))");
        exit(EXIT_FAILURE);
    }

    // Now, we make the processes.

    for (int i = 0; i < file_count; ++i) {
        if (fork() == 0) {
            // We're at the child process now.
            // We look for the word in the ith process.
            // We then exit appropriately.
            exit(search_file(argv[i + 2], word) == WORD_FOUND ?
                 WORD_FOUND : WORD_NOT_FOUND);
        }
    }

    for (int i = 0; i < file_count; ++i) {
        int child_status;
        wait(&child_status);
        if (WIFEXITED(child_status)) {
        // I hear switches are much faster than ifs.
            switch (WEXITSTATUS(child_status)) {
                case WORD_FOUND:
                    printf("Word %s found in %s.\n",
                           word, argv[i + 2]);
                    break;
                case WORD_NOT_FOUND:
                    printf("Word %s not found in %s.\n",
                           word, argv[i + 2]);
                    break;
                case FILE_NOT_FOUND:
                    printf("ERROR: %s: no such file.\n",
                           argv[i + 2]);
                    break;
                default:
                    fprintf(stderr, 
                            "ERROR: unknown error occured (exit status: %d).\n"
                            "Contact your system adminstrator.\n",
                            WEXITSTATUS(child_status));
                    break;
            }
        }
    }

    return EXIT_SUCCESS;
}
