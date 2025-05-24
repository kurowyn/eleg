#ifndef UTILS_H
#define UTILS_H

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wait.h>

#if defined(STATIC_READ)
    #define MAX_BUFFER_LEN 1024
#endif

#if defined(VERBOSE_ERR_HANDLE) 
    #define ERR_HANDLE(err_cond, err_msg, err_handle, ...) \
        if (err_cond) {fprintf(stderr, err_msg, __VA_ARGS__); err_handle;}
#elif !defined(VERBOSE_ERR_HANDLE) || defined(SILENT_ERR_HANDLE)
    #define ERR_HANDLE(err_cond, err_msg, err_handle, ...) \
        if (err_cond) {err_handle;}
#endif 

#define MIN_ARG_COUNT 3

// The NO_STATUS one actually makes sense, since it means something went so
// wrong that we couldn't even check whether the word is even in the file.
// Also used when we intialize the FileStatusList - we haven't yet done 
// anything.
enum {NO_STATUS = -1,
      WORD_FOUND = 0,
      WORD_NOT_FOUND = 1,
      FILE_NOT_FOUND = ENOENT,};
// Check errno manpage for the last one.

typedef struct FileStatus FileStatus;

struct FileStatus {
    int8_t status;
    char *name;
};

FileStatus *FileStatusListInit(char **file_list, uint64_t file_count);

// Might replace this with getline soon.
char *dynamic_read(const char *file_name);

int search_file(const char *file_name, const char *token);

#endif // UTILS_H
