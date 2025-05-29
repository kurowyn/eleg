#include "eleg.h" 

int
FileSearch(const char *file_name, const char *target){
    char buffer[MAX_BUFFER_LEN];
    FILE *f = fopen(file_name, "r");

    ERR_HANDLE(f == NULL,
               "ERROR: could not open file %s, errno: %d\n",
               return errno,
               file_name, errno);

    while (fgets(buffer, MAX_BUFFER_LEN, f)) {
        if (buffer && strstr(buffer, target)) {
            return FILE_WORD_FOUND;
        }
    }
    
    return FILE_WORD_NOT_FOUND;
}

void
FileShowStatusList(const char *target, 
                   const FileStatus *file_status_list, 
                   const ssize_t file_count){
    for (ssize_t i = 0; i < file_count; ++i) {
        switch (file_status_list[i].status) {
            case FILE_WORD_FOUND:
                fprintf(stdout, "@%d: %s: %s found.\n", 
                        i + 1, file_status_list[i].name, target);
                break;
            case FILE_WORD_NOT_FOUND:
                fprintf(stdout, "@%d: %s: %s not found.\n", 
                        i + 1, file_status_list[i].name, target);
                break;
            case FILE_NOT_FOUND:
                fprintf(stdout, "@%d: %s: no such file.\n",
                        i + 1, file_status_list[i].name);
                break;
            default:
                fprintf(stderr, "ERROR: unkown error occurred.\n"
                                "File status: %d, errno: %d.\n"
                                "Contact your system adminstrator.\n",
                                file_status_list[i].status, errno);
                break;
        }
    }
}

FileStatus *
FileStatusListInit(char **file_list, ssize_t file_count){
    FileStatus *file_status_list = malloc_shared(file_count * sizeof (FileStatus));

    ERR_HANDLE(file_status_list == MAP_FAILED,
               "ERROR: could not mmap, errno: %d\n",
               exit(errno),
               errno); 

    for (ssize_t i = 0; i < file_count; ++i) {
        file_status_list[i] = (FileStatus){.name = file_list[i],
                                           .status = FILE_NO_STATUS};
    }

    return file_status_list;
}
