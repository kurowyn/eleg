#ifndef UTILS_H
#define UTILS_H

#include <errno.h>
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

enum {WORD_FOUND = 0,
      WORD_NOT_FOUND = 1,
      FILE_NOT_FOUND = ENOENT};
// Check errno manpage.

char *dynamic_read(const char *filename);

int search_file(const char *filename, const char *token);

#endif // UTILS_H
