#include "utils.h"

// This may be removed in the future, in favor of getline.
char *
dynamic_read(const char *file_name){
    char *buffer = malloc(0);
    FILE *f = fopen(file_name, "r");

    ERR_HANDLE(buffer == NULL, 
               "ERROR: could not allocate memory, errno: %d",
               return NULL,
               errno);

    ERR_HANDLE(f == NULL,
               "ERROR: could not open file %s, errno: %d\n",
               return NULL,
               file_name, errno);

    for (;;) {
        static int i = 0, c;
        if ((c = fgetc(f)) != EOF) {
            // This is for the unlikely event we encounter 
            // NULL characters in our file.
            if (c != '\0') {
                buffer = realloc(buffer, (++i) * sizeof (char));

                ERR_HANDLE(buffer == NULL, 
                           "ERROR: could not reallocate memory, errno: %d",
                           return NULL,
                           errno);

                buffer[i - 1] = c;
            } 
        } else {
                buffer[i - 1] = '\0';
                break;
        }
    }

    fclose(f);

    return buffer;
}

int 
search_file(const char *file_name, const char *token){
#if !defined(STATIC_READ) || defined(DYNAMIC_READ)
    char *buffer = dynamic_read(file_name);

    ERR_HANDLE(buffer == NULL,
               "ERROR: NULL buffer, errno: %d\n",
               return errno,
               errno);

    int found = (strstr(buffer, token) != NULL) ? WORD_FOUND : WORD_NOT_FOUND;

    free(buffer);

    return found;
#elif defined(STATIC_READ)
    FILE *f = fopen(file_name, "r");

    ERR_HANDLE(f == NULL,
               "ERROR: could not open file %s, errno: %d\n",
               return errno,
               file_name, errno);

    char buffer[MAX_BUFFER_LEN];

    while (fgets(buffer, MAX_BUFFER_LEN, f) != NULL) {
        if (strstr(buffer, token) == NULL) {
            continue;
        }
        return WORD_FOUND;
    }

    fclose(f);

    return WORD_NOT_FOUND;
#endif
}

// The number of files in filelist MUST match filecount.
// The behavior is otherwise completely unpredictable.
// I think that means this function should be private.
FileStatus *FileStatusListInit(char **file_list, uint64_t file_count){
    FileStatus *FileStatusList = mmap(NULL,
                                      file_count * sizeof (FileStatus),       
                                      PROT_READ | PROT_WRITE,
                                      MAP_SHARED | MAP_ANONYMOUS,
                                      -1,
                                      0);

    // Keeping it at 0 (the default) can cause WORD_FOUND collisions.
    for (size_t i = 0; i < file_count; ++i) {
        FileStatusList[i] = (FileStatus){.status = NO_STATUS, 
                                         .name = file_list[i]};
    }

    return FileStatusList;
}
