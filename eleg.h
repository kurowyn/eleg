#ifndef ELEG_H
#define ELEG_H

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wait.h>

// Enough for everyone.
#define MAX_BUFFER_LEN 4096

#define STR_EQ(str_1, str_2) (strcmp((str_1), (str_2)) == 0)

#if defined(VERBOSE_ERR_HANDLE) 
    #define ERR_HANDLE(err_cond, err_msg, err_handle, ...) \
        if (err_cond) {fprintf(stderr, (err_msg), __VA_ARGS__); err_handle;}
#elif !defined(VERBOSE_ERR_HANDLE) || defined(SILENT_ERR_HANDLE)
    #define ERR_HANDLE(err_cond, err_msg, err_handle, ...) \
        if (err_cond) {err_handle;}
#endif 

#define malloc_shared(size)  \
        mmap(NULL, \
             size, \
             PROT_READ | PROT_WRITE, \
             MAP_SHARED | MAP_ANONYMOUS, \
             -1, \
             0) \

// A wrapper around fgets that makes life easier.
#define STRING_INPUT(prompt_format, string, continue_condition, ...) \
{ \
    do { \
        printf(prompt_format, __VA_ARGS__); \
        fgets(string, MAX_BUFFER_LEN, stdin); \
    } while ((continue_condition)); \
    if (string[strlen(string) - 1] == '\n' && (string[strlen(string) - 1] = '\0')); \
} \

#define CLEAN_UP(argv, file_status_list) \
    munmap(file_status_list, FILE_COUNT * sizeof (FileStatus)); \
    if (_g_malloc_used) { \
        for (ssize_t i = 0; i < argc; ++i) { \
            free(argv[i]); \
        } \
        free(argv); \
    } \

enum {FILE_NO_STATUS = -1,
      FILE_WORD_FOUND = 0,
      FILE_WORD_NOT_FOUND = 1,
      FILE_NOT_FOUND = ENOENT,};
// ENOENT is indeed 2 (in most implementations of errno), but that's hard to see. 

typedef struct FileStatus FileStatus;
struct FileStatus {
    int status; // whether we found the target
    char *name; // name of the file
};

FileStatus *FileStatusListInit(char **file_list, ssize_t file_count);
int FileSearch(const char *file_name, const char *target);
void FileShowStatusList(const char *target,
                        const FileStatus *file_status_list,
                        const ssize_t file_count);

extern int _g_malloc_used;

#endif // ELEG_H
