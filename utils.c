#include "utils.h"

char *
dynamic_read(const char *filename){
    char *buffer = malloc(0);
    FILE *f = fopen(filename, "r");

    ERR_HANDLE(buffer == NULL, 
               "ERROR: could not allocate memory, errno: %d",
               return NULL,
               errno);

    ERR_HANDLE(f == NULL,
               "ERROR: could not open file %s, errno: %d\n",
               return NULL,
               filename, errno);

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
search_file(const char *filename, const char *token){
#if !defined(STATIC_READ) || defined(DYNAMIC_READ)
    char *buffer = dynamic_read(filename);

    if (buffer == NULL)
        return errno;

    int found = (strstr(buffer, token) != NULL) ? WORD_FOUND : WORD_NOT_FOUND;

    free(buffer);

    return found;
#elif defined(STATIC_READ)
    FILE *f = fopen(filename, "r");

    ERR_HANDLE(f == NULL,
               "ERROR: could not open file %s, errno: %d\n",
               return errno,
               filename, errno);

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
